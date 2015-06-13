#include "prefix.h"

#include "../src/pool_dict_tree.h"

typedef PoolDictTree::Node Node;
typedef PoolDictTree::Tag Tag;
typedef PoolDictTree::Edge Edge;

TEST (PoolDictTreeTest, Extending) {
    Buffer input;
    BufferCharWriter writer(input);
    writer.put("ab abcd abde");
    //          ^  ^    ^
    //          0  3    8

    PoolDictTree t(input);
    Node const* root = t.root();
    ASSERT_TRUE(root->is_leaf());
    ASSERT_EQ(Tag(true, 0, 0, 0), root->tag);

    t.extend(0, -int('a') - 1, 1);
    // (0)-a-(1)

    ASSERT_EQ(1, root->child_cnt());
    Edge _a = t.edge(root, 'a');
    ASSERT_EQ("a", _a);
    Node const* a = _a.dst;
    ASSERT_TRUE(a->is_leaf());
    ASSERT_EQ(Tag(true, 1, -int('a') - 1, 1), a->tag);

    t.extend(1, 0, 2);
    // (0)-a-(1)-b-(2)

    ASSERT_EQ(1, root->child_cnt());
    ASSERT_EQ(_a, t.edge(root, 'a'));
    ASSERT_EQ(1, a->child_cnt());
    Edge a_b = t.edge(a, 'b');
    ASSERT_EQ("b", a_b);
    Node const* ab = a_b.dst;
    ASSERT_TRUE(ab->is_leaf());
    ASSERT_EQ(Tag(true, 2, 0, 2), ab->tag);

    t.extend(2, 3, 1);
    // (0)-a-b-(2)-c-(1)

    ASSERT_EQ(1, root->child_cnt());
    Edge _ab = t.edge(root, 'a');
    ASSERT_EQ("ab", _ab);
    ASSERT_EQ(ab, _ab.dst);
    ASSERT_EQ(1, ab->child_cnt());
    Edge ab_c = t.edge(ab, 'c');
    ASSERT_EQ("c", ab_c);
    Node const* abc = ab_c.dst;
    ASSERT_TRUE(abc->is_leaf());
    ASSERT_EQ(Tag(true, 2, 0, 2), ab->tag);
    ASSERT_EQ(Tag(true, 1, 3, 3), abc->tag);

    t.extend(2, 8, 3);
    // (0)-a-b-(2)-c-(1)
    //          |
    //          d
    //          |
    //         (3)

    ASSERT_EQ(1, root->child_cnt());
    ASSERT_EQ(_ab, t.edge(root, 'a'));
    ASSERT_EQ(2, ab->child_cnt());
    ASSERT_EQ(ab_c, t.edge(ab, 'c'));
    ASSERT_TRUE(abc->is_leaf());
    Edge ab_d = t.edge(ab, 'd');
    ASSERT_EQ("d", ab_d);
    Node const* abd = ab_d.dst;
    ASSERT_TRUE(abd->is_leaf());
    ASSERT_EQ(Tag(true, 2, 0, 2), ab->tag);
    ASSERT_EQ(Tag(true, 1, 3, 3), abc->tag);
    ASSERT_EQ(Tag(true, 3, 8, 3), abd->tag);

    t.extend(1, 3, 2);
    // (0)-a-b-(*)-c-(1)-d-(2)
    //          |
    //          d
    //          |
    //         (3)

    ASSERT_EQ(1, root->child_cnt());
    ASSERT_EQ(_ab, t.edge(root, 'a'));
    ASSERT_EQ(2, ab->child_cnt());
    ASSERT_EQ(ab_c, t.edge(ab, 'c'));
    ASSERT_EQ(ab_d, t.edge(ab, 'd'));
    ASSERT_TRUE(abd->is_leaf());
    ASSERT_EQ(1, abc->child_cnt());
    Edge abc_d = t.edge(abc, 'd');
    ASSERT_EQ("d", abc_d);
    Node const* abcd = abc_d.dst;
    ASSERT_TRUE(abcd->is_leaf());
    ASSERT_EQ(Tag(false, 2, 0, 2), ab->tag);
    ASSERT_EQ(Tag(true , 1, 3, 3), abc->tag);
    ASSERT_EQ(Tag(true , 3, 8, 3), abd->tag);
    ASSERT_EQ(Tag(true , 2, 3, 4), abcd->tag);

    t.extend(0, 0, 3);
    // (0)-a-(3)-b-c-(1)-d-(2)

    ASSERT_EQ(1, root->child_cnt());
    _a = t.edge(root, 'a');
    ASSERT_EQ("a", _a);
    a = _a.dst;
    ASSERT_EQ(1, a->child_cnt());
    Edge a_bc = t.edge(a, 'b');
    ASSERT_EQ("bc", a_bc);
    ASSERT_EQ(abc, a_bc.dst);
    ASSERT_EQ(1, abc->child_cnt());
    ASSERT_EQ(abc_d, t.edge(abc, 'd'));
    ASSERT_TRUE(abcd->is_leaf());
    ASSERT_EQ(Tag(true, 3, 0, 1), a->tag);
    ASSERT_EQ(Tag(true, 1, 3, 3), abc->tag);
    ASSERT_EQ(Tag(true, 2, 3, 4), abcd->tag);
}
