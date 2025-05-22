#include "executioner.hpp"
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
void Executioner::reap() { waitpid(-1, nullptr, 0); }

bool Executioner::exec(std::string cmd, std::vector<char *> &args) {
  auto res = internal(args);
  if (!res) {
    return external(args);
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

bool Executioner::external(std::vector<char *> &cmd) {
  pid_t pid = fork();
  if (pid == -1) {
    std::cerr << "fork failed" << std::endl;
    return false;
  }
  if (pid == 0) {
    int errcode = execvp(cmd[0], cmd.data());
    if (errcode == -1) {
      if (errno == ENOENT) {
        std::cout << cmd[0] << ": command not found" << std::endl;
        exitCode_ = 127;
        return false;
      } else {
        std::cout << strerror(errno) << std::endl;
        exitCode_ = 126;
        return false;
      }
    }
  } else {
    pids_.push_back(pid);
  }
  return true;
}
int Executioner::getExitCode() const { return exitCode_; }
int Executioner::getPsCount() const { return pids_.size(); }
