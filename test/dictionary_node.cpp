#include "prefix.hpp"

#include "../src/dictionary_node.hpp"

TEST(DictionaryNode, InitializesProperly) {
    DictionaryNode node1(5);
    EXPECT_EQ(nullptr, node1.parent());
    EXPECT_EQ(0, node1.child_cnt());
    EXPECT_EQ(5, node1.codeword_no());
    EXPECT_TRUE(node1.is_active());

    DictionaryNode node2(0);
    EXPECT_EQ(0, node2.codeword_no());

    DictionaryNode node3(-3);
    EXPECT_EQ(0, node3.codeword_no());
}

TEST(DictionaryNode, LinksProperly) {
    DictionaryNode root(0);
    DictionaryNode node1(1);
    DictionaryNode node2(2);

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

TEST(DictionaryNode, MaintainsActiveState) {
    DictionaryNode root(0);
    DictionaryNode node(5);
    root.link_child('a', &node);

    EXPECT_TRUE(node.is_active());
    node.deactivate();
    EXPECT_FALSE(node.is_active());
}
