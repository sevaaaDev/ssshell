#ifndef EXECUTIONER_HPP
#define EXECUTIONER_HPP

#include <vector>
class Executioner {
public:
  /*exit code of previous call*/
  int exitCode{};

public:
  /*@return false if should quit*/
  bool exec(std::vector<char *> &cmd);
  bool execWithPipe(std::vector<char *> &, std::vector<char *> &);

private:
  bool external(std::vector<char *> &cmd);
};

#endif // !EXECUTIONER_HPP
