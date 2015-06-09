#include "prefix.h"

#include "../src/word_tree_node.h"

TEST (WordTreeNodeTest, Initialization) {
    WordTreeNode<int> node(5);
    EXPECT_EQ(nullptr, node.parent());
    EXPECT_EQ(0, node.child_cnt());
    EXPECT_EQ(5, node.tag);
}

TEST (WordTreeNodeTest, Linking) {
    WordTreeNode<int> root(0);
    WordTreeNode<int> node1(1);
    WordTreeNode<int> node2(2);

    root.link_child('a', &node1);
    EXPECT_EQ(1, root.child_cnt());
    EXPECT_EQ(&root, node1.parent());
    EXPECT_EQ(&node1, root.child('a'));

    root.link_child('b', &node2);
    EXPECT_EQ(2, root.child_cnt());
    EXPECT_EQ(&root, node1.parent());
    EXPECT_EQ(&root, node2.parent());
    EXPECT_EQ(&node1, root.child('a'));
    EXPECT_EQ(&node2, root.child('b'));

    node1.link_child('c', &node2);
    EXPECT_EQ(1, root.child_cnt());
    EXPECT_EQ(1, node1.child_cnt());
    EXPECT_EQ(&root, node1.parent());
    EXPECT_EQ(&node1, node2.parent());
    EXPECT_EQ(&node1, root.child('a'));
    EXPECT_EQ(&node2, node1.child('c'));
}
