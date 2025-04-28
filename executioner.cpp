#include "executioner.hpp"
#include <iostream>
#include <string.h>
#include <string_view>
#include <unistd.h>
#include <vector>
#include <wait.h>

bool Executioner::exec(std::vector<char *> &cmd) {
  std::string_view str = cmd[0];
  if (str == "exit") {
    return false;
  }
  return external(cmd);
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
        exitCode = 127;
        return false;
      } else {
        std::cout << strerror(errno) << std::endl;
        exitCode = 126;
        return false;
      }
    }
  } else {
    // NOTE: parent
    int status;
    pid_t wpid = waitpid(-1, &status, 0);
    if (WIFEXITED(status)) {
      exitCode = WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
      exitCode = WTERMSIG(status) + 128;
    }
  }
  return true;
}
