#ifndef BUILTIN_COMMAND_HPP
#define BUILTIN_COMMAND_HPP
#define STOP_LOOP false
#define KEEP_LOOP true
#include <unistd.h>
#include <vector>
struct CmdResult {
  int exitCode;
  bool loopAgain;
};

inline CmdResult exit(std::vector<char *> &_) { return {-1, STOP_LOOP}; }
inline CmdResult cd(std::vector<char *> &args) {
  int exitCode = 0;
  if (chdir(args[1]) == -1) {
    exitCode = 1;
  }
  return {exitCode, KEEP_LOOP};
}

#endif // !BUILTIN_COMMAND_HPP
