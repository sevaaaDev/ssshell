#include "../src/parser.hpp"
#include "./ut.hpp"
#include <string>
#include <vector>

using namespace boost::ut;
using namespace boost::ut::spec;
std::string formatTree(Node &node) {
  std::string str;
  if (node.token != nullptr) {
    str += node.token->string;
  }
  if (node.token->type != TKN_STRING) {
    str += " (";
  }
  for (auto child : node.children) {
    str += formatTree(child) + " ";
  }
  if (node.token->type != TKN_STRING) {
    str += ")";
  }
  return str;
}
suite<"parser_test"> parser_test = [] {
  describe("parse cmd") = [] {
    it("should return 1 node for 1 cmd") = [] {
      std::vector<Token> tokens;
      tokens.push_back({.type = TKN_STRING, .string = "ls"});
      Parser parser(tokens);
      Node root = parser.parsing(0);
      auto fmt = formatTree(root);
      expect(fmt == "ls") << fmt;
    };
    it("should return tree of 2 cmd") = [] {
      std::vector<Token> tokens;
      tokens.push_back({.type = TKN_STRING, .string = "ls"});
      tokens.push_back({.type = TKN_AND, .string = "&&"});
      tokens.push_back({.type = TKN_STRING, .string = "ls"});
      Parser parser(tokens);
      Node root = parser.parsing(0);
      auto fmt = formatTree(root);
      expect(fmt == "&& (ls ls )") << fmt;
    };
    it("should return tree of multiple cmd") = [] {
      std::vector<Token> tokens;
      tokens.push_back({.type = TKN_STRING, .string = "ls"});
      tokens.push_back({.type = TKN_AND, .string = "&&"});
      tokens.push_back({.type = TKN_STRING, .string = "ls"});
      tokens.push_back({.type = TKN_PIPE, .string = "|"});
      tokens.push_back({.type = TKN_STRING, .string = "ls"});
      tokens.push_back({.type = TKN_SEMICOLON, .string = ";"});
      tokens.push_back({.type = TKN_STRING, .string = "ls"});
      Parser parser(tokens);
      Node root = parser.parsing(0);
      auto fmt = formatTree(root);
      expect(fmt == "; (&& (ls | (ls ls ) ) ls )") << fmt;
    };
  };
};
