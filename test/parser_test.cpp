#include "../src/parser.hpp"
#include "./ut.hpp"

int main() {
  using namespace boost::ut;
  using namespace boost::ut::spec;
  describe("parserCMD") = [] {
    it("should parse &&") = [] {
      std::string cmd = "ls && echo";
      auto vecOfCmd = Parser::parseInput(cmd);
      expect(vecOfCmd.size() == 3_i);
      expect(std::string_view(vecOfCmd[0].cmd) == "ls");
      expect(std::string_view(vecOfCmd[2].cmd) == "echo");
      expect(vecOfCmd[1].separator == S_AND);
    };
    it("should parse ||") = [] {
      std::string cmd = "ls || echo";
      auto vecOfCmd = Parser::parseInput(cmd);
      expect(std::string_view(vecOfCmd[0].cmd) == "ls");
      expect(std::string_view(vecOfCmd[2].cmd) == "echo");
      expect(vecOfCmd[1].separator == S_OR);
    };
    it("should parse ;") = [] {
      std::string cmd = "ls ; echo";
      auto vecOfCmd = Parser::parseInput(cmd);
      expect(std::string_view(vecOfCmd[0].cmd) == "ls");
      expect(std::string_view(vecOfCmd[2].cmd) == "echo");
      expect(vecOfCmd[1].separator == S_SEMICOLON);
    };
  };
}
