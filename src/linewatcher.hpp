#ifndef LINEWATCHER_HPP
#define LINEWATCHER_HPP

#include "history.hpp"
#include <string>
#include <termios.h>
class Linewatcher {
public:
  Linewatcher(History &history) : history_(history) {}
  std::string getline();
  bool isEOF();

private:
  History &history_;
  bool isEOF_ = false;
  termios origTermios_;
};

#endif // !LINEWATCHER_HPP
