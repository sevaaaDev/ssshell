#include "linewatcher.hpp"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <termios.h>
#include <unistd.h>

bool Linewatcher::isEOF() { return isEOF_; }

static void printBuf(const std::deque<char> &buf) {
  std::cout << "\x1b[1K\r";
  std::flush(std::cout);
  for (auto c : buf) {
    std::cout << c;
  }
  std::flush(std::cout);
}
enum KEYS {
  ESC,
  ARW_UP,
  ARW_DOWN,
  ARW_LEFT,
  ARW_RIGHT,
};

static KEYS parseSeq() {
  char seq[2];
  if (read(STDIN_FILENO, &seq[0], 1) != 1) {
    return ESC;
  }
  if (read(STDIN_FILENO, &seq[1], 1) != 1) {
    return ESC;
  }
  if (seq[1] == 'A') {
    return ARW_UP;
  }
  if (seq[1] == 'B') {
    return ARW_DOWN;
  }
  return ESC;
}
std::string Linewatcher::getline() {
  std::deque<char> buf{};
  std::deque<char> bufZero{};
  std::deque<char>::iterator cursor = buf.end();
  char c;
  int historyIndex = 0;
  isEOF_ = false;
  while (1) {
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    if (c == '\n') {
      std::cout << std::endl;
      break;
    }
    if (c == '\x1b') {
      KEYS key = parseSeq();
      if (key == ARW_UP) {
        if (std::abs(historyIndex) == history_.getSize()) {
          continue;
        }
        if (historyIndex == 0) {
          bufZero = buf;
        }
        buf = history_.at(--historyIndex);
        cursor = buf.end();
        printBuf(buf);
      }
      if (key == ARW_DOWN) {
        if (historyIndex >= 0) {
          continue;
        }
        if (historyIndex == -1) {
          historyIndex = 0;
          buf = bufZero;
          cursor = buf.end();
          printBuf(buf);
          continue;
        }
        buf = history_.at(++historyIndex);
        cursor = buf.end();
        printBuf(buf);
      }
      c = '\0';
    }
    if (c == 127 && !buf.empty()) {
      historyIndex = 0;
      buf.pop_back();
      printBuf(buf);
      continue;
    }
    if (c == 4) {
      isEOF_ = true;
      break;
    }
    if (c != '\0') {
      buf.insert(buf.end(), c);
      printBuf(buf);
    }
  }
  std::string str{};
  if (!isEOF_) {
    str = {buf.begin(), buf.end()};
  }
  if (buf.size()) {
    history_.push(buf);
  }
  return str;
}
