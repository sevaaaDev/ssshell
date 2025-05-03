#ifndef TREE_HPP
#define TREE_HPP
#include "parser.hpp"
#include <vector>
inline Node *getParent(std::vector<Node> &vecOfCmd, int lastIndex) {
  if (lastIndex == 0) {
    return &vecOfCmd[0];
  }
  Node &parent = vecOfCmd.at(lastIndex - 1);
  parent.right = &vecOfCmd.at(lastIndex);
  parent.left = getParent(vecOfCmd, lastIndex - 2);
  return &parent;
}
inline Node *constructTree(std::vector<Node> &vecOfCmd) {
  // for (int i = vecOfCmd.size() - 2; i > 0; i = i - 2) {
  //   Node &parent = vecOfCmd.at(i);
  //   parent.right = &vecOfCmd.at(i + 1);
  //   parent.left = &vecOfCmd.at(i - 1);
  // }
  // return &vecOfCmd.back();
  return getParent(vecOfCmd, vecOfCmd.size() - 1);
}

// 0 cmd
// 1 sep
// 2 cmd
// 3 sep
// 4 cmd
// 5 sep
// 6 cmd
//
//
// parent = 3
// left = getParent(2)
// right = 4
//
// getparent(last max index = i)
//   if i = 0
//      return arr[0]
//   parent = arr[i - 1]
//   left = getparent(i - 2);
//   right = arr[i]
//   return parent
//
#endif // !TREE_HPP
