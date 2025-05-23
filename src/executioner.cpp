#include "executioner.hpp"
#include <deque>
#include <iostream>
#include <optional>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <wait.h>

void Executioner::wait(pid_t pid) {
  int status;
  pid_t wpid = waitpid(pid, &status, 0);
  if (WIFEXITED(status)) {
    exitCode_ = WEXITSTATUS(status);
  }
  if (WIFSIGNALED(status)) {
    exitCode_ = WTERMSIG(status) + 128;
  }
}
void Executioner::wait() {
  if (pids_.empty())
    return;
  pid_t pid = pids_.back();
  pids_.pop_back();
  wait(pid);
}
void Executioner::reap() {
  waitpid(-1, nullptr, 0);
  pids_.pop_back();
}

bool Executioner::exec(std::string cmd, std::vector<char *> &args, FDpair fd) {
  auto res = internal(args);
  if (!res) {
    return external(args, fd);
  } else {
    return res.value();
  }
}

std::optional<bool> Executioner::internal(std::vector<char *> &cmd) {
  // TODO: does map store its key?
  // if not then we can use str view instead
  if (builtin_.count(cmd[0]) == 1) {
    auto result = builtin_[cmd[0]](cmd);
    if (result.exitCode >= 0) {
      exitCode_ = result.exitCode;
    }
    return result.loopAgain;
  }
  return std::nullopt;
}

bool Executioner::external(std::vector<char *> &cmd, FDpair fd) {
  pid_t pid = fork();
  if (pid == -1) {
    std::cerr << "fork failed" << std::endl;
    return false;
  }
  if (pid == 0) {
    // if write or read exist, dup
    // close fd
    if (fd.write) {
      dup2(fd.write.value(), STDOUT_FILENO);
    }
    if (fd.read) {
      dup2(fd.read.value(), STDIN_FILENO);
    }
    closePipeFD();
    int errcode = execvp(cmd[0], cmd.data());
    if (errcode == -1) {
      if (errno == ENOENT) {
        std::cout << "sssh: " << cmd[0] << ": command not found" << std::endl;
        exitCode_ = 127;
        return false;
      } else {
        std::cout << "sssh: " << strerror(errno) << std::endl;
        exitCode_ = 126;
        return false;
      }
    }
  } else {
    pids_.push_back(pid);
  }
  return true;
}

void Executioner::closePipeFD() {
  for (FDpair &pair : pipefds_) {
    if (pair.read)
      close(pair.read.value());
    if (pair.write)
      close(pair.write.value());
  }
  pipefds_.clear();
}

void Executioner::setupPipeFD(int count) {
  std::deque<int> fd;
  for (int i = 0; i < count; i++) {
    int pipefd[2];
    pipe(pipefd);
    fd.push_back(pipefd[1]);
    fd.push_back(pipefd[0]);
  }
  for (int i = 0; i < count + 1; i++) {
    if (i == 0) {
      pipefds_.push_back({.write = fd.front()});
      fd.pop_front();
      continue;
    }
    int r = fd.front();
    fd.pop_front();
    if (fd.empty()) {
      pipefds_.push_back({.read = r});
      continue;
    }
    int w = fd.front();
    fd.pop_front();
    pipefds_.push_back({w, r});
  }

  // 1
  // fd.size = 2
  //
}
const std::deque<FDpair> &Executioner::getPipeFDs() const { return pipefds_; }
int Executioner::getExitCode() const { return exitCode_; }
int Executioner::getPsCount() const { return pids_.size(); }
