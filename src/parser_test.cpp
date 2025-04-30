#include "parser.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

int main() {
  using namespace std::literals;
  {
    std::cout << "parse 'ls -la'" << std::endl;
    std::string str = "ls -la";
    Parser parser(str);
    std::vector<char *> parsed = parser.parseCmd(str);
    assert(parsed.size() == 2);
    assert(*parsed[0] == 'l');
    assert(*parsed[1] == '-');
    std::cout << "Success" << std::endl;
  }
  {
    std::cout << "parse '  ls    -la'" << std::endl;
    std::string str = "  ls    -la";
    Parser parser(str);
    std::vector<char *> parsed = parser.parseCmd(str);
    assert(parsed.size() == 2);
    assert(*parsed[0] == 'l');
    assert(std::string_view(parsed[1]) == "-la"sv);
    std::cout << "Success" << std::endl;
  }
  {
    std::cout << "get first cmd 'ls -la ; echo hell'" << std::endl;
    std::string str = "ls -la ; echo hell";
    Parser parser(str);
    std::vector<char *> parsed = parser.getNextCmd();
    assert(std::string_view(parsed[1]) == "-la"sv);
    std::cout << "Success" << std::endl;
  }
  {
    std::cout << "get second cmd 'ls -la ; echo hell'" << std::endl;
    std::string str = "ls -la ; echo hell";
    Parser parser(str);
    parser.getNextCmd();
    std::vector<char *> parsed = parser.getNextCmd();
    assert(std::string_view(parsed[1]) == "hell"sv);
    std::cout << "Success" << std::endl;
  }
  {
    std::cout << "get empty vector when command empty" << std::endl;
    std::string str = "ls -la ";
    Parser parser(str);
    parser.getNextCmd();
    std::vector<char *> parsed = parser.getNextCmd();
    assert(parsed.size() == 0);
    std::cout << "Success" << std::endl;
  }
}
