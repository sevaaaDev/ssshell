#include "../src/parser.hpp"
#include "./ut.hpp"

using namespace boost::ut;
using namespace boost::ut::spec;
suite<"parser_test"> parser_test = [] {
  describe("parserCMD") = [] {
    it("should parse &&") = [] {
      std::string cmd = "ls && echo";
      auto vecOfCmd = parser::getCommands(cmd);
      expect(vecOfCmd.size() == 3_i);
      expect(std::string_view(vecOfCmd[0].cmd) == "ls ");
      expect(std::string_view(vecOfCmd[2].cmd) == " echo");
      expect(vecOfCmd[1].separator == S_AND);
    };
    it("should parse ||") = [] {
      std::string cmd = "ls || echo";
      auto vecOfCmd = parser::getCommands(cmd);
      expect(std::string_view(vecOfCmd[0].cmd) == "ls ");
      expect(std::string_view(vecOfCmd[2].cmd) == " echo");
      expect(vecOfCmd[1].separator == S_OR);
    };
    it("should parse ;") = [] {
      std::string cmd = "ls ; echo";
      auto vecOfCmd = parser::getCommands(cmd);
      expect(std::string_view(vecOfCmd[0].cmd) == "ls ");
      expect(std::string_view(vecOfCmd[2].cmd) == " echo");
      expect(vecOfCmd[1].separator == S_SEMICOLON);
    };
  };
  describe("parse args") = [] {
    it("should parse whitespace separated") = [] {
      std::string cmd = "ls -la";
      auto args = parser::getArgs(cmd);
      expect(std::string_view(args[0]) == "ls");
      expect(std::string_view(args[1]) == "-la");
    };
    it("should parse trim leading and trailing space") = [] {
      std::string cmd = "  ls -la  ";
      auto args = parser::getArgs(cmd);
      expect(std::string_view(args[0]) == "ls");
      expect(std::string_view(args[1]) == "-la");
    };
    it("should parse ignore inbetween space") = [] {
      std::string cmd = "  ls     -la  ";
      auto args = parser::getArgs(cmd);
      expect(std::string_view(args[0]) == "ls");
      expect(std::string_view(args[1]) == "-la");
    };
  };
};
