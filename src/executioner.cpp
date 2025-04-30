#include "executioner.hpp"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <wait.h>

bool Executioner::exec(std::string cmd, std::vector<char *> &args) {
  // TODO: does map store its key?
  // if not then we can use str view instead
  if (builtin_.count(cmd) == 1) {
    auto result = builtin_[cmd]();
    if (result.exitCode >= 0) {
      exitCode_ = result.exitCode;
    }
    return result.loopAgain;
  }
  return external(args);
}

bool Executioner::external(std::vector<char *> &cmd) {
  pid_t pid = fork();
  if (pid == -1) {
    std::cerr << "fork failed" << std::endl;
    return false;
  }
  if (pid == 0) {
    // NOTE: child
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
      // WARN: should exit? if cmd queue exist this will keep looping. try
      // handle it in exec call
    }
  } else {
    // NOTE: parent
    int status;
    pid_t wpid = waitpid(-1, &status, 0);
    if (WIFEXITED(status)) {
      exitCode_ = WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
      exitCode_ = WTERMSIG(status) + 128;
    }
  }
  return true;
}
