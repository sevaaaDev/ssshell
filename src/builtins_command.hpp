#ifndef BUILTIN_COMMAND_HPP
#define BUILTIN_COMMAND_HPP
#include <string_view>
#define STOP_LOOP false
#define KEEP_LOOP true
#include <cstdlib>
#include <unistd.h>
#include <vector>
struct CmdResult {
  int exitCode;
  bool loopAgain;
};

inline CmdResult exit(std::vector<char *> &_) { return {-1, STOP_LOOP}; }
inline char *getTargetDir(std::vector<char *> &args) {
  char *targetDir = nullptr;
  if (args.size() == 1) {
    targetDir = std::getenv("HOME");
  } else {
    targetDir = args[1];
  }
  return targetDir;
}
inline CmdResult cd(std::vector<char *> &args) {
  int exitCode = 0;
  char *targetDir = getTargetDir(args);
  if (chdir(targetDir) == -1) {
    exitCode = 1;
  }
  return {exitCode, KEEP_LOOP};
}

#endif // !BUILTIN_COMMAND_HPP
