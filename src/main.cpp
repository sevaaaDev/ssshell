#include "executioner.hpp"
#include "parser.hpp"
#include "tree.hpp"
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
bool execTree(Executioner &executioner, Node &node) {
  if (node.cmd) {
    auto cmd = Parser::getArgs(node.cmd);
    return executioner.exec(cmd[0], cmd);
  }
  bool run = execTree(executioner, *node.left);
  if (run == false) {
    return run;
  }
  if (node.separator == S_SEMICOLON) {
    return execTree(executioner, *node.right);
  }
  if (node.separator == S_AND && executioner.exitCode_ == 0) {
    return execTree(executioner, *node.right);
  }
  if (node.separator == S_OR && executioner.exitCode_ != 0) {
    return execTree(executioner, *node.right);
  }
  return run;
}
int main() {
  Executioner executioner;
  Linewatcher linewatcher;
  bool run = false;
  do {
    std::string input = linewatcher.getline(executioner.exitCode_);
    if (linewatcher.isEOF_) {
      std::cout << std::endl;
      break;
    }
    auto vecOfCmd = Parser::getCommands(input);
    // construct tree
    Node *root = constructTree(vecOfCmd);
    run = execTree(executioner, *root);
    // TODO: we use ptr and ref randomly, please pick one or try to use it in
    // make sense scenario
  } while (run);
  return executioner.exitCode_;
}
