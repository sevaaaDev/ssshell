#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <string_view>
#include <vector>
enum TokenType {
  TKN_STRING,
  TKN_SEMICOLON,
  TKN_AND,
  TKN_OR,
  TKN_PIPE,
  TKN_BG,
};

struct Token {
  TokenType type;
  std::string string;
};

class Lexer {

public:
  Lexer(std::string_view input) : input_(input) {}
  std::vector<Token> getTokens();

private:
  std::string_view input_;
};

#endif // !LEXER_HPP
