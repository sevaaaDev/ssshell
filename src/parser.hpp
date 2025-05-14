#ifndef PARSER_HPP
#define PARSER_HPP
#include "lexer.hpp"
#include <deque>
#include <string_view>
#include <vector>

enum Separator {
  S_NONE,
  S_AND,
  S_OR,
  S_SEMICOLON,
};
struct Node {
  Token *token = nullptr;
  std::vector<Node> children{};
};

class Parser {
public:
  Parser(std::vector<Token> &tokens) : tokens_(tokens) {};
  Node parsing(int minBindingPower);

private:
  std::vector<Token> &tokens_;
  int i_ = 0;
  Token &next();
  Token &peek();
};
namespace parser {
std::vector<char *> getArgs(const std::string_view);
std::vector<Node> getCommands(const std::vector<Token>);
std::vector<Node> parsing(int minBindingPower);
}; // namespace parser

#endif // !PARSER_HPP
// getline
// vec of node = parser.input(getline.input);
// construct tree (vec of node)
// // USE NAMESPACE FOR PARSER
// execTree(tree)
//
// struct Node {
//    std::string separator;
//    std::vector<char*> cmd;
//    Node left;
//    Node right;
// }
//
//          ||
//         /  \
//       &&    cmd
//      /  \
//    cmd  cmd
//
// && ||
//
//
// execTree(node) {
//    if (cmd == null) {
//      execTree(node.left);
//      if (sep is && and exit is success) {
//         execTree(node.right);
//      }
//      else if (sep is || and exit is err) {
//         execTree(node.right);
//      }
//      else if (sep is ;){
//         execTree(node.right);
//      }
//      return;
//    }
//    exec(parser(cmd));
//    return void
// }
//
//
// "ls -la && echo hell || echo worl"
// Node ls -la
// Node &&
// Node echo hell
// Node ||
// Node echo worl
//
// for i = last-1; i > 0; i =i -2 {
//    parent = v.at(i)
//    parent.right = v.at(last)
//    parent.left = v.at(i - 2)
// }
//
