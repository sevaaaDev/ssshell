#include "executioner.hpp"
#include "executioner_utils.hpp"
#include "history.hpp"
#include "lexer.hpp"
#include "linewatcher.hpp"
#include "parser.hpp"
#include <iostream>
#include <memory>
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

std::string getCWD() {
  auto ptr = std::make_unique<char *>(get_current_dir_name());
  return *ptr.get();
}

int main() {
  enableRaw();
  History history;
  Executioner executioner;
  Linewatcher linewatcher(history);
  bool run = false;
  do {
    std::string input = linewatcher.getline(
        getCWD() + " " + std::to_string(executioner.getExitCode()) + "> ");
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
