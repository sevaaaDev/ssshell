#include "parser.hpp"
#include <string_view>
#include <vector>

std::vector<Node> Parser::parseInput(const std::string_view inputBuf) {
  std::vector<Node> parsed;
  for (int i = 0, k = i; i < inputBuf.size(); i++) {
    char *c = const_cast<char *>(&inputBuf[i]);
    if (inputBuf[i] == ';') {
      *c = '\0';
      k = i + 1;
      parsed.push_back({.separator = SEMICOLON});
    }
    if (k == i) {
      parsed.push_back({.cmd = c});
    }
  }
  return parsed;
}

std::vector<char *> Parser::parseCmd(std::string_view cmd) {
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
