// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <misc/tree.hpp>
#include <boost/bind.hpp>
#include <boost/coroutine/all.hpp>

typedef ajd::binary_tree<char> tree;
typedef boost::coroutines::coroutine<char()> generator;

bool is_leaf(tree::node l) { return !(l->left || l->right); }

void next_leaf(generator::caller_type& yield, tree::node& node)
{
  if(node) {
    next_leaf(yield, node->left);
    next_leaf(yield, node->right);
    if(is_leaf(node)) { yield(node->value); }
  }
}

bool same_fringe(tree::node one, tree::node two)
{
  generator leaf1(boost::bind(next_leaf, _1, one));
  generator leaf2(boost::bind(next_leaf, _1, two));
  for(; leaf1 && leaf2; leaf1(), leaf2()) {// iterate till *both* traversal are active
    if(leaf1.get() != leaf2().get()) {// compare the next leaf node
      return false;// return false, if they don't match.
    }
  }
  if(leaf1 || leaf2) {// if one of the traversals is still active,
    return false;// one tree has more leaves than the other.
  }
  return true;
}

int main()
{
  tree::node empty = tree::read_tree("");
  tree::node red = tree::read_tree("(-(-(-(X)(R))(-(I)(N)))(-(G)(E)))");
  tree::node blue1 = tree::read_tree("(-(-(-(F)(R))(-(I)(N)))(-(G)(E)))");
  tree::node blue2 = tree::read_tree("(-(-(-(-(-(F)(R))(I))(N))(G))(E))");
  tree::node blue3 = tree::read_tree("(-(-(-(-()(F))(R))(I))(-(-(-(N)())(G))(E))");
  tree::node diff = tree::read_tree("(-(-(-(-()(F))(R))(I))(-(-(-(N)())(G))())");
  assert(!same_fringe(empty, red));
  assert(!same_fringe(red, blue1));
  assert(same_fringe(empty, empty));
  assert(same_fringe(blue1, blue2));
  assert(same_fringe(blue2, blue3));
  assert(!same_fringe(blue2, diff));
}
