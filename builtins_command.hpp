#ifndef BUILTIN_COMMAND_HPP
#define BUILTIN_COMMAND_HPP

#define STOP_LOOP false
struct CmdResult {
  int exitCode;
  bool loopAgain;
};

inline CmdResult exit() { return {-1, STOP_LOOP}; }

#endif // !BUILTIN_COMMAND_HPP
