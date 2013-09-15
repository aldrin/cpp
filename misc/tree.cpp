/// Copyright (c) 2013, Aldrin's Notebook.
/// http://opensource.org/licenses/BSD-2-Clause

#include "tree.hpp"

#include <sstream>
#include <cassert>
#include <iostream>

namespace ajd
{
  typedef binary_tree<char> char_tree;
  typedef char_tree::node ctree;

  static void postorder(ctree root, std::ostream &out)
  {
    if (root)
    {
      postorder(root->left, out);
      postorder(root->right, out);
      out << root->value;
    }
  }

  ctree from_preorder_inorder(const std::string &preorder, const std::string &inorder)
  {
    assert(preorder.size() == inorder.size());

    if (preorder.empty()) { return ctree(); }

    std::string::size_type r = inorder.find(preorder[0]);

    std::string left_inorder(inorder, 0, r);
    std::string left_preorder(preorder, 1, r);

    std::string right_inorder(inorder, r + 1);
    std::string right_preorder(preorder, r + 1);

    ctree root = char_tree::make_node(preorder[0]);
    root->left = from_preorder_inorder(left_preorder, left_inorder);
    root->right = from_preorder_inorder(right_preorder, right_inorder);

    return root;
  }

  void test_traversals()
  {
    ctree t(from_preorder_inorder("FBADCEGIH", "ABCDEFGHI"));
    std::stringstream stream;
    postorder(t, stream);
    assert(stream.str() == "ACEDBHIGF");
  }

  void test_reads()
  {
    assert(!char_tree::read_tree("()"));
    ctree one = char_tree::read_tree("(A)");
    assert(one && one->value == 'A');
    ctree two = char_tree::read_tree("(A()(C))");
    assert(!two->left);
    assert(two->value == 'A');
    assert(two->right->value == 'C');
    ctree three = char_tree::read_tree("(A(B)(C))");
    assert(three->value == 'A');
    assert(three->left->value == 'B');
    assert(three->right->value == 'C');
    ctree four = char_tree::read_tree("(A (B () (D)) (C (E) ()) )");
    assert(four->value == 'A');
    assert(four->left->value == 'B');
    assert(!four->left->left);
    assert(four->left->right->value == 'D');
    assert(four->right->value == 'C');
    assert(four->right->left->value == 'E');
    assert(!four->right->right);


    typedef binary_tree<int> int_tree;
    typedef int_tree::node itree;
    itree five = int_tree::read_tree("(0 (-1) (1000 () (123)))");
    assert(five->value==0);
    assert(five->left->value==-1);
    assert(five->right->value==1000);
    assert(!five->right->left);
    assert(five->right->right->value==123);
  }
}


int main(int argc, char **argv)
{
  ajd::test_traversals();
  ajd::test_reads();
}
