#ifndef EXECUTIONER_HPP
#define EXECUTIONER_HPP

#include "builtins_command.hpp"
#include <string>
#include <unordered_map>
#include <vector>
using BuiltinFunc = CmdResult (*)(std::vector<char *> &args);
class Executioner {
public:
  /*exit code of previous call*/
  int exitCode_{};

public:
  /*@return false if should quit*/
  bool exec(std::string, std::vector<char *> &);
  bool execWithPipe(std::vector<char *> &, std::vector<char *> &);

private:
  std::unordered_map<std::string, BuiltinFunc> builtin_ = {{"exit", &exit},
                                                           {"cd", &cd}};
  bool external(std::vector<char *> &cmd);
};

#endif // !EXECUTIONER_HPP
