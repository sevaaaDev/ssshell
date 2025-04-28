#include "executioner.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>
#include <vector>

std::string getline() {
  std::string buf;
  std::cout << "ssshell> ";
  std::getline(std::cin, buf, '\n');
  // TODO: if eof should exit without exec
  // if (std::cin.eof()) {
  //   exit(0);
  // }
  return buf;
}
int main() {
  Executioner executioner;
  bool loop = false;
  do {
    std::string input = getline();
    Parser parser(input);
    std::vector<char *> cmd = parser.getNextCmd();
    loop = executioner.exec(cmd);
  } while (loop);
  return executioner.exitCode;
}
