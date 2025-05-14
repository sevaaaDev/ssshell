#include "lexer.hpp"
#include <vector>

std::vector<Token> Lexer::getTokens() {
  enum State {
    ST_START,
    ST_COMPLETE,
    ST_STOP,
    ST_STRING,
    ST_AND_SYM,
    ST_PIPE_SYM,
    ST_SEMICOLON,
  };
  State stateNow = ST_START;
  State stateNext = ST_START;
  TokenType tokenType = TKN_STRING;
  std::string tokenString;
  std::vector<Token> tokens;
  int i = 0;
  while (i < input_.length() || stateNow != ST_STOP) {
    switch (stateNow) {
    case ST_START:
      if (input_[i] == '&') {
        stateNext = ST_AND_SYM;
      } else if (input_[i] == '|') {
        stateNext = ST_PIPE_SYM;
      } else if (input_[i] == ';') {
        stateNext = ST_SEMICOLON;
      } else {
        stateNext = ST_STRING;
      }
      tokenString.push_back(input_[i]);
      ++i;
      break;
    case ST_AND_SYM:
      if (input_[i] == '&') {
        tokenType = TKN_AND;
        tokenString.push_back(input_[i]);
        ++i;
      } else {
        tokenType = TKN_BG;
      }
      stateNext = ST_COMPLETE;
      break;
    case ST_PIPE_SYM:
      if (input_[i] == '|') {
        tokenType = TKN_OR;
        tokenString.push_back(input_[i]);
        ++i;
      } else {
        tokenType = TKN_PIPE;
      }
      stateNext = ST_COMPLETE;
      break;
    case ST_SEMICOLON:
      tokenType = TKN_SEMICOLON;
      stateNext = ST_COMPLETE;
      break;
    case ST_STRING:
      if (i < input_.length() &&
          !(input_[i] == '&' || input_[i] == '|' || input_[i] == ';')) {
        stateNext = ST_STRING;
        tokenString.push_back(input_[i]);
        ++i;
      } else {
        tokenType = TKN_STRING;
        stateNext = ST_COMPLETE;
      }
      break;
    case ST_COMPLETE:
      tokens.push_back({.type = tokenType, .string = tokenString});
      tokenString.clear();
      stateNext = ST_START;
      if (i >= input_.length()) {
        stateNext = ST_STOP;
      }
      break;
    case ST_STOP:
      break;
    }
    stateNow = stateNext;
  }
  return tokens;
}
