#ifndef EXECUTIONER_HPP
#define EXECUTIONER_HPP

#include "builtins_command.hpp"
#include <string>
#include <unordered_map>
#include <vector>
class Executioner {
public:
  /*exit code of previous call*/
  int exitCode_{};

public:
  /*@return false if should quit*/
  bool exec(std::vector<char *> &cmd);
  bool execWithPipe(std::vector<char *> &, std::vector<char *> &);

private:
  std::unordered_map<std::string, CmdResult (*)(void)> builtin_ = {
      {"exit", &exit}};
  bool external(std::vector<char *> &cmd);
};

#endif // !EXECUTIONER_HPP
