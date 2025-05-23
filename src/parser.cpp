#include "parser.hpp"
#include "lexer.hpp"
#include <string_view>
#include <vector>

std::string Node::print() {
  std::string str = "(";
  if (token != nullptr) {
    str += token->string;
  }
  for (auto child : children) {
    str += child.print();
  }
  str += ")";
  return str;
}
void Node::flat() {
  if (token == nullptr || token->type != TKN_PIPE)
    return;
  std::vector<Node> newChildren;
  for (Node &child : children) {
    if (child.token != nullptr && child.token->type != TKN_PIPE) {
      newChildren.push_back(child);
    }
    if (child.token != nullptr && child.token->type == TKN_PIPE) {
      child.flat();
      for (Node &grandChild : child.children) {
        newChildren.push_back(grandChild);
      }
    }
  }
  children = newChildren;
}
Token *Parser::next() {
  if (i_ >= tokens_.size()) {
    return nullptr;
  };
  return &tokens_[i_++];
}

int Parser::peek(std::vector<TokenType> types) {
  if (i_ >= tokens_.size()) {
    return 0;
  }
  for (auto type : types) {
    if (tokens_[i_].type == type) {
      return 1;
    }
  }
  return 0;
}

Node Tree(Node lhs, Node root, Node rhs) {
  root.children.push_back(lhs);
  root.children.push_back(rhs);
  return root;
}

Node Parser::E(int *err) {
  Node lhs = T(err);
  if (*err) {
    return lhs;
  }
  Node op;
  Node rhs;
  while (peek({TKN_AND, TKN_OR, TKN_SEMICOLON})) {
    op = {.token = next()};
    rhs = T(err);
    if (*err) {
      return rhs;
    }
    lhs = Tree(lhs, op, rhs);
  }
  return lhs;
}

Node Parser::T(int *err) {
  Node lhs = F(err);
  if (*err) {
    return lhs;
  }
  Node op;
  Node rhs;
  while (peek({TKN_PIPE})) {
    op = {.token = next()};
    rhs = F(err);
    if (*err) {
      return rhs;
    }
    lhs = Tree(lhs, op, rhs);
    lhs.flat();
  }
  return lhs;
}
Node Parser::F(int *err) {
  int peekStatus = peek({TKN_STRING});
  if (!peekStatus) {
    *err = 1;
  }
  Node lhs = {.token = next()};
  // if lhs is not string, error
  return lhs;
}
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
  parsed.push_back(nullptr);
  return parsed;
}
