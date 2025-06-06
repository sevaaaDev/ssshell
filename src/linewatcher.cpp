#include "linewatcher.hpp"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <termios.h>
#include <unistd.h>

bool Linewatcher::isEOF() { return isEOF_; }

static void printBuf(const std::deque<char> &buf) {
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
inline void printSameLine(std::string_view text) {
  std::cout << "\x1b[1K\r";
  std::cout << text;
  std::flush(std::cout);
}
std::string Linewatcher::getline(const std::string &prompt) {
  std::deque<char> buf{};
  std::deque<char> bufZero{};
  std::deque<char>::iterator cursor = buf.end();
  isEOF_ = false;
  printSameLine(prompt);
  char c = '\0';
  while (1) {
    int nread = read(STDIN_FILENO, &c, 1);
    if (nread == 0) continue;
    if (c == '\n') {
      std::cout << std::endl;
      break;
    }
    if (c == 4) {
      isEOF_ = true;
      break;
    }
    switch (c) {
    case 127:
      if (!buf.empty()) {
        history_.resetIndex();
        buf.pop_back();
      }
      break;
    case '\x1b': {
      KEYS key = parseSeq();
      if (key == ARW_UP) {
        if (history_.index() == -1) {
          bufZero = buf;
        }
        std::deque<char> histBuf = history_.prev();
        if (!histBuf.empty()) {
          buf = histBuf;
          cursor = buf.end();
        }
      }
      if (key == ARW_DOWN) {
        std::deque<char> histBuf = history_.next();
        if (histBuf.empty()) {
          buf = bufZero;
        } else {
          buf = histBuf;
        }
        cursor = buf.end();
      }
      break;
    }
    default:
      buf.insert(buf.end(), c);
    }
    printSameLine(prompt);
    printBuf(buf);
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
// if (c == 127 && !buf.empty()) {
//   history_.resetIndex();
//   buf.pop_back();
// }
// if (c == '\x1b') {
//   KEYS key = parseSeq();
//   if (key == ARW_UP) {
//     if (history_.index() == -1) {
//       bufZero = buf;
//     }
//     std::deque<char> histBuf = history_.prev();
//     if (!histBuf.empty()) {
//       buf = histBuf;
//     }
//     cursor = buf.end();
//     print(prompt);
//     printBuf(buf);
//   }
//   if (key == ARW_DOWN) {
//     std::deque<char> histBuf = history_.next();
//     if (histBuf.empty()) {
//       buf = bufZero;
//     } else {
//       buf = histBuf;
//     }
//     cursor = buf.end();
//     print(prompt);
//     printBuf(buf);
//   }
//   c = '\0';
// }
// if (c != '\0') {
//   buf.insert(buf.end(), c);
//   print(prompt);
//   printBuf(buf);
// }
