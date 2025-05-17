#include "executioner.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <vector>

// TODO: shell exit code should be accesible to other class or func
// set it global?

class Linewatcher {
public:
  std::string makePrompt(int exitCode) {
    std::unique_ptr<char> cwd(getcwd(nullptr, 0));
    return std::string(cwd.get()) + " " + std::to_string(exitCode) + " > ";
  }
  bool isEOF_ = false;
  std::string getline(int exitCode) {
    std::string buf;
    std::cout << makePrompt(exitCode);
    std::getline(std::cin, buf, '\n');
    if (std::cin.eof()) {
      isEOF_ = true;
    }
    return buf;
  }
};
bool execTree(Executioner &executioner, Node &node) {
  if (node.token->type == TKN_STRING) {
    auto cmd = parser::getArgs(node.token->string);
    return executioner.exec(cmd[0], cmd);
  }
  bool run = execTree(executioner, node.children[0]);
  if (run == false) {
    return run;
  }
  if (node.token->type == TKN_SEMICOLON) {
    return execTree(executioner, node.children[1]);
  }
  if (node.token->type == TKN_AND && executioner.exitCode_ == 0) {
    return execTree(executioner, node.children[1]);
  }
  if (node.token->type == TKN_OR && executioner.exitCode_ == 1) {
    return execTree(executioner, node.children[1]);
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
    Lexer lexer(input);
    auto tokens = lexer.getTokens();
    Parser parser(tokens);
    int err = 0;
    Node root = parser.E(&err);
    // if err

    // "ls -la || echo hello" linewatcher
    // {STRING, ls -la} {OR, ||} {STRING, echo hello} lexer
    // tree of token parser
    // char * []= tree.left.token.string.map(' ' -> '\0') final lexer / args
    // exec
    run = execTree(executioner, root);
    // TODO: we use ptr and ref randomly, please pick one or try to use it in
    // make sense scenario
  } while (run);
  return executioner.exitCode_;
}
