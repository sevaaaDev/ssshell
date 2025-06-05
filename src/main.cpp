#include "executioner.hpp"
#include "executioner_utils.hpp"
#include "history.hpp"
#include "lexer.hpp"
#include "linewatcher.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>

termios origTermios;
void enableRaw() {
  tcgetattr(STDIN_FILENO, &origTermios);
  termios raw = origTermios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
void disableRaw() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios); }

int main() {
  enableRaw();
  History history;
  Executioner executioner;
  Linewatcher linewatcher(history);
  bool run = false;
  do {
    std::string input = linewatcher.getline();
    if (linewatcher.isEOF()) {
      std::cout << std::endl;
      break;
    }
    if (input.empty()) {
      run = true;
      continue;
    }
    Lexer lexer(input);
    auto tokens = lexer.getTokens();
    Parser parser(tokens);
    int err = 0;
    Node root = parser.E(&err);
    run = Exec::execTree(executioner, root);
    executioner.wait();
  } while (run);
  disableRaw();
  return executioner.getExitCode();
}
