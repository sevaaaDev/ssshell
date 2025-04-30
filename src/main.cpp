#include "executioner.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>
#include <vector>

// TODO: shell exit code should be accesible to other class or func
// set it global?

class Linewatcher {
public:
  bool isEOF_ = false;
  std::string getline(int exitCode) {
    std::string buf;
    std::cout << "ssshell " << exitCode << " > ";
    std::getline(std::cin, buf, '\n');
    if (std::cin.eof()) {
      isEOF_ = true;
    }
    return buf;
  }
};
int main() {
  Executioner executioner;
  Linewatcher linewatcher;
  bool loop = false;
  do {
    std::string input = linewatcher.getline(executioner.exitCode_);
    if (linewatcher.isEOF_) {
      std::cout << std::endl;
      break;
    }
    Parser parser(input);
    std::vector<char *> cmd = parser.getNextCmd();
    loop = executioner.exec(cmd);
  } while (loop);
  return executioner.exitCode_;
}
