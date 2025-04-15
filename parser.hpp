#ifndef PARSER_HPP
#define PARSER_HPP
#include <deque>
#include <string_view>
#include <vector>

class Parser {
  std::deque<char *> parseInput(const std::string_view);
  std::deque<char *> commands;

public:
  Parser(const std::string_view);
  std::vector<char *> parseCmd(const std::string_view);
  std::vector<char *> getNextCmd();
};

#endif // !PARSER_HPP
