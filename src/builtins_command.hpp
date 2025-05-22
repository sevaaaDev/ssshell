#ifndef BUILTIN_COMMAND_HPP
#define BUILTIN_COMMAND_HPP
#include <cstdlib>
#include <string>
#include <string_view>
#include <unistd.h>
#include <vector>
#define STOP_LOOP false
#define KEEP_LOOP true
struct CmdResult {
  int exitCode;
  bool loopAgain;
};

inline CmdResult exit(std::vector<char *> &args) {
  int exitCode = -1;
  // if (args.size() >= 2) {
  //   exitCode = std::stoi(args[1]);
  // }
  return {exitCode, STOP_LOOP};
}
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
