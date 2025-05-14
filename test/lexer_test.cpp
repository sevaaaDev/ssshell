#include "../src/lexer.hpp"
#include "./ut.hpp"
#include <vector>

using namespace boost::ut;
using namespace boost::ut::spec;

std::string formatTokens(std::vector<Token> &tokens) {
  std::string str;
  for (auto token : tokens) {
    str += '[' + token.string + ']';
  }
  return str;
}
suite<"lexer"> lex = [] {
  describe("lexer") = [] {
    it("should generate token for 1 cmd") = [] {
      std::string input = "ls -la";
      Lexer lex(input);
      std::vector<Token> tokens = lex.getTokens();
      std::string fmt = formatTokens(tokens);
      expect(fmt == "[ls -la]") << fmt;
    };
    it("should generate token for multiple cmd") = [] {
      std::string input = "ls;echo && sleep | foo || bar";
      Lexer lex(input);
      std::vector<Token> tokens = lex.getTokens();
      std::string fmt = formatTokens(tokens);
      expect(fmt == "[ls][;][echo ][&&][ sleep ][|][ foo ][||][ bar]") << fmt;
    };
    it("should generate token for random string") = [] {
      std::string input = "|||Hell&&&;;;and";
      Lexer lex(input);
      std::vector<Token> tokens = lex.getTokens();
      std::string fmt = formatTokens(tokens);
      expect(fmt == "[||][|][Hell][&&][&][;][;][;][and]") << fmt;
    };
  };
};
