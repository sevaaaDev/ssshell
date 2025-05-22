#ifndef EXECUTIONER_HPP
#define EXECUTIONER_HPP

#include "builtins_command.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
using BuiltinFunc = CmdResult (*)(std::vector<char *> &args);
class Executioner {
public:
  /*@return false if should quit*/
  bool exec(std::string, std::vector<char *> &);
  void wait(pid_t pid);
  void wait();
  void reap();
  int getExitCode() const;
  int getPsCount() const;

private:
  std::unordered_map<std::string, BuiltinFunc> builtin_ = {{"exit", &exit},
                                                           {"cd", &cd}};
  bool external(std::vector<char *> &cmd);
  std::optional<bool> internal(std::vector<char *> &cmd);
  /*exit code of previous call*/
  int exitCode_{};
  std::vector<pid_t> pids_;
};
//
// exec();
// exec();
// wait();
// waitAll();
//
// // NOTE: parent
// int status;
// pid_t wpid = waitpid(-1, &status, 0);
// if (WIFEXITED(status)) {
//   exitCode_ = WEXITSTATUS(status);
// }
// if (WIFSIGNALED(status)) {
//   exitCode_ = WTERMSIG(status) + 128;
// }

#endif // !EXECUTIONER_HPP
