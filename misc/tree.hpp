/// Copyright (c) 2013, Aldrin's Notebook.
/// http://opensource.org/licenses/BSD-2-Clause

#pragma once

#include <stack>
#include <memory>
#include <sstream>

namespace ajd
{
  /// A simple binary tree structure to try out algorithm puzzles.
  template<typename T> struct binary_tree
  {
    typedef std::shared_ptr<binary_tree> node;

    T value;
    node left;
    node right;

    /// A static factory method for a node.
    static node make_node(T v) { return std::shared_ptr<binary_tree>(new binary_tree(v)); }

    /// A static builder method for a tree from a string input. The input is expected to be a fully
    /// parenthesized expressions of the form:

    /// tree  := '(' value tree tree ')' | '()'
    /// value := any string that can be converted to T

    /// Examples: (), (A () (B)), (-1 (0 (1000 (2) (123)) ()) ())

    /// This code is for solving algorithm-puzzles. It does the right thing when given the right
    /// input. There is limited input validation and no error reporting, please mind your inputs.
    static node read_tree(std::string input)
    {
      // trim.
      input.erase(input.find_last_not_of(" )") + 1);
      input.erase(0, input.find_first_not_of(" ("));

      // if no input, return null node.
      if (input.empty()) { return node(); }

      // everything upto the first parenthesis is the value of the root node.
      size_t i(0);
      std::stringstream stream;
      while (i < input.length() && input.at(i) != '(') { stream << input.at(i++); }
      T rootval = T();
      stream >> rootval;
      node root = make_node(rootval);

      // if we have input left, we make left and right subtrees.
      if (i < input.length())
      {
        size_t left_start(i);
        std::stack<char> stack;
        stack.push(input.at(i)); // input[i] has to be '('
        while (i < input.length() && !stack.empty()) // loop till we close the first '('
        {
          char next = input.at(++i);
          if (next == '(') {stack.push(next);}
          if (next == ')') {stack.pop(); }
        }
        size_t left_end = i;
        root->left = read_tree(input.substr(left_start, left_end - left_start + 1));
        root->right = read_tree(input.substr(left_end + 1));
      }

      return root;
    }

  private:
    binary_tree(T v): value(v) {}
  };
}
