#include "../src/tree.hpp"
#include "./ut.hpp"
#include <vector>

using namespace boost::ut;
using namespace boost::ut::spec;
suite<"tree_test"> tree_test = [] {
  describe("construct tree") = [] {
    std::vector<Node> v;
    v.push_back({.cmd = nullptr});
    v.push_back({.separator = S_SEMICOLON});
    v.push_back({.cmd = nullptr});
    v.push_back({.separator = S_SEMICOLON});
    v.push_back({.cmd = nullptr});
    expect(fatal(v.size() == 5_ul)) << "v is not 5";
    Node *root = constructTree(v);
    it("should give the same node from the vec") = [&root, &v] {
      expect(root == &v[3]);
    };
    expect(fatal(v.size() == 5_ul)) << "v is not 5";
    it("should have right child") = [&root, &v] {
      expect(root->right == &v[4]);
    };
    it("should have left child") = [&root, &v] { expect(root->left == &v[1]); };
  };
};
