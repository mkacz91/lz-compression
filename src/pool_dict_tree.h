#ifndef POOL_DICT_TREE_H
#define POOL_DICT_TREE_H

#include "prefix.h"

#include "buffer.h"
#include "word_tree_node.h"

// PoolDictTree
// =============================================================================
// TODO: documentation
class PoolDictTree {
public:
    // TODO doc
    // Each node of the tree used to effectively find codeword matches contains
    // information about its active, the corresponding codeword number, and
    // the `begin` and `length` properties defining that codeword.
    struct Tag {
        bool active;
        int codeword_no;
        int begin;
        int length;

        Tag (bool active, int codeword_no, int begin, int length);

        friend bool operator == (Tag const& tag1, Tag const& tag2);
    };
    typedef WordTreeNode<Tag> Node;

    // TODO doc
    class Edge {
    public:
        Node const* dst;

        int length () const;

        char operator [] (int i) const;

    private:
        BufferCharSlice m_slice;

        Edge (Node const* dst, BufferCharSlice const& slice);

        friend class PoolDictTree;
        friend bool operator == (Edge const& e1, Edge const& e2);
        friend bool operator == (string const& s, Edge const& e);
    };

    // Constructs a new dictionary tree built upon given buffer, consisting only
    // of the root node.
    PoolDictTree (Buffer const& input);

    ~PoolDictTree ();

    // Extend the `i`th codeword by one letter and assign index `j` to the
    // resulting codeword. The extending letter is determined based on `begin`
    // which is the starting position of the new codeword within the buffer.
    //
    // There are two possible scenarios depending on the value of `j`:
    //
    //   * If `j` is an index of an already exisiting node, that node gets
    //     discarded in favour of the new one.
    //
    //   * If `j` is equal to the number of codewords plus one, a fresh codeword
    //     is allocated.
    //
    // That way a contiguous range of codeword numbers is always maintained.
    void extend (int i, int begin, int j);

    // TODO doc
    Edge edge (Node const* node, char a) const;

    // TODO doc
    Edge edge_to_root () const;

private:

    // The underlying buffer.
    Buffer const& m_input;

    // A buffer consisting of all possible characters in order. Used to take
    // slices from when providing egdges to single letter codewords, if present.
    Buffer m_alphabet;

    // Root node.
    Node* m_root;

    // An array mapping codeword indices to tree nodes.
    std::vector<Node*> m_nodes;

    // Removes a node from the tree. Implicitly called by `extend()`.
    void remove (Node* node);

    // TODO doc
    BufferCharSlice slice (int begin, int length) const;
};

inline PoolDictTree::Edge PoolDictTree::edge_to_root () const {
    return Edge(m_root, BufferCharSlice());
}

// PoolDictBase::Tag
// =============================================================================

inline
PoolDictTree::Tag::Tag (bool active, int codeword_no, int begin, int length) :
    active(active),
    codeword_no(codeword_no),
    begin(begin),
    length(length)
{
    assert(codeword_no >= 0);
    assert(length >= 0);
}

inline bool
operator == (PoolDictTree::Tag const& tag1, PoolDictTree::Tag const& tag2) {
    return tag1.active == tag2.active
        && tag1.codeword_no == tag2.codeword_no
        && tag1.begin == tag2.begin
        && tag2.length == tag2.length;
}

// PoolDictTree::Edge
// =============================================================================

inline int PoolDictTree::Edge::length () const {
    return m_slice.length();
}

inline char PoolDictTree::Edge::operator [] (int i) const {
    return m_slice[i];
}

inline bool operator == (
    PoolDictTree::Edge const& e1,
    PoolDictTree::Edge const& e2
) {
    return e1.dst == e2.dst && e1.m_slice == e2.m_slice;
}

inline bool operator == (string const& s, PoolDictTree::Edge const& e) {
    return s == e.m_slice;
}

#endif // POOL_DICT_TREE_H
