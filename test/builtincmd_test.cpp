#include "../src/builtins_command.hpp"
#include "./ut.hpp"
#include <string_view>
#include <vector>
using namespace boost::ut;
using namespace boost::ut::spec;
suite<"builtins_command"> builtin_test = [] {
  describe("cd") = [] {
    it("should cd to targetdir") = [] {
      std::vector<char *> args = {"cd", "/home"};
      char *target = getTargetDir(args);
      expect(std::string_view(target) == "/home");
    };
    it("should cd to home when not given targetdir") = [] {
      std::vector<char *> args = {"cd"};
      char *target = getTargetDir(args);
      expect(std::string_view(target) == "/home/main");
    };
  };
};
