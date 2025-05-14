#include "parser.hpp"
#include <string_view>
#include <vector>

Node Parser::parsing(int minBindingPower) {
  Node lhs = {.token = &(next())};
  // TODO: refactor the if statement
  // peek() should handle if there are no element left
  if (i_ >= tokens_.size()) {
    return lhs;
  }
  while (peek().bp() > minBindingPower) {
    Node op = {.token = &(next())};
    Node rhs = parsing(op.token->bp());
    op.children.push_back(lhs);
    op.children.push_back(rhs);
    lhs = op;
    if (i_ >= tokens_.size()) {
      break;
    }
  }
  return lhs;
}

Token &Parser::next() { return tokens_[i_++]; }
Token &Parser::peek() { return tokens_[i_]; }

std::vector<char *> parser::getArgs(std::string_view cmd) {
  std::vector<char *> parsed;
  for (int i = 0, k = i; i < cmd.size(); i++) {
    char *c = const_cast<char *>(&cmd[i]);
    if (cmd[i] == ' ') {
      *c = '\0';
      k = i + 1;
    }
    if (k == i) {
      parsed.push_back(c);
    }
  }
  return parsed;
}
