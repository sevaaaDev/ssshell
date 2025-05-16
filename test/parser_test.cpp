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
  describe("Node") = [] {
    it("should print correctly") = [] {
      Token c1 = {.type = TKN_STRING, .string = "ls"};
      Token c2 = {.type = TKN_STRING, .string = "ls"};
      Token op = {.type = TKN_AND, .string = "&&"};
      Node root = {.token = &op};
      root.children.push_back({.token = &c1});
      root.children.push_back({.token = &c2});
      expect(root.print() == "(&&(ls)(ls))") << root.print();
    };
  };
  describe("parse cmd") = [] {
    it("should parse 1 token") = [] {
      std::vector<Token> tokens;
      Token c1 = {.type = TKN_STRING, .string = "ls"};
      // Token c2 = {.type = TKN_STRING, .string = "ls"};
      // Token op = {.type = TKN_AND, .string = "&&"};
      tokens.push_back(c1);
      // tokens.push_back(op);
      // tokens.push_back(c2);
      Parser parser(tokens);
      int e = 0;
      Node root = parser.E(&e);
      expect(root.print() == "(ls)") << root.print();
      expect(e == 0_i);
    };
    it("should parse multi token") = [] {
      std::vector<Token> tokens;
      Token c1 = {.type = TKN_STRING, .string = "ls"};
      Token c2 = {.type = TKN_STRING, .string = "ls"};
      Token op = {.type = TKN_AND, .string = "&&"};
      tokens.push_back(c1);
      tokens.push_back(op);
      tokens.push_back(c2);
      Parser parser(tokens);
      int e = 0;
      Node root = parser.E(&e);
      expect(root.print() == "(&&(ls)(ls))") << root.print();
      expect(e == 0_i);
    };
    it("should parse multi token with precedence") = [] {
      std::vector<Token> tokens;
      Token c1 = {.type = TKN_STRING, .string = "ls"};
      Token c2 = {.type = TKN_STRING, .string = "ls"};
      Token op = {.type = TKN_AND, .string = "&&"};
      tokens.push_back(c1);
      tokens.push_back(op);
      tokens.push_back(c2);
      tokens.push_back({.type = TKN_PIPE, .string = "|"});
      tokens.push_back({.type = TKN_STRING, .string = "echo"});
      Parser parser(tokens);
      int e = 0;
      Node root = parser.E(&e);
      expect(root.print() == "(&&(ls)(|(ls)(echo)))") << root.print();
      expect(e == 0_i);
    };
    it("should err when unexpected token") = [] {
      std::vector<Token> tokens;
      Token c1 = {.type = TKN_AND, .string = "&&"};
      tokens.push_back(c1);
      Parser parser(tokens);
      int e = 0;
      Node root = parser.E(&e);
      expect(root.print() == "(&&)") << root.print();
      expect(e == 1_i);
    };
    it("should err when token not found") = [] {
      std::vector<Token> tokens;
      Token c1 = {.type = TKN_STRING, .string = "ls"};
      Token op = {.type = TKN_AND, .string = "&&"};
      tokens.push_back(c1);
      tokens.push_back(op);
      Parser parser(tokens);
      int e = 0;
      Node root = parser.E(&e);
      expect(root.print() == "()") << root.print();
      expect(e == 2_i);
    };
  };
};
