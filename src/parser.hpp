#ifndef PARSER_HPP
#define PARSER_HPP
#include <deque>
#include <string_view>
#include <vector>

class Parser {
  std::deque<char *> commands;

public:
  Parser(const std::string_view);
  std::vector<char *> parseCmd(const std::string_view);
  std::vector<char *> getNextCmd();
  std::deque<char *> parseInput(const std::string_view);
};

#endif // !PARSER_HPP
