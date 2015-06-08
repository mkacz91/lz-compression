#ifndef ENCODE_DICT_TREE_H
#define ENCODE_DICT_TREE_H

#include "prefix.h"

#include <vector>

// TODO: documentation
class EncodeDictTree {
public:
    // Constructs a tree with given underlying buffer and containing only the
    // root, i.e., the empty codeword, at index 0.
    EncodeDictTree (Buffer const& input);

    ~EncodeDictTree ();

    // Extend the `i`th codeword by one letter and assign index `j` to the
    // resulting codeword. The extending letter is determined based on `begin`
    // which is the starting position of the new codeword within the buffer.
    //
    // There are two possible scenarios depending on the value of `j`:
    //
    //   * If `j` is an index of an already exisiting node, that node gets
    //     discarded in favour og the new one.
    //
    //   * If `j` is equal to the number of codewords plus one, a fresh codeword
    //     is allocated.
    //
    // That way a contiguous range of codeword numbers is always maintained.
    void extend (int i, int begin, int j);

    // Returns the subsequence of the input buffer that corresponds to the
    // edge from codeword `i` along character `a`.
    BufferCharSlice edge (int i, char a);

    // Returns the codeword with number `i`.
    Codeword const& codeword (int i) const;

private:
    // The tree node. Extends `EncodeDictNode` with information about active
    // state and the starting position of the corresponding word within the
    // input buffer.
    class Node : public EncodeDictNode {
    public:
        bool active;
        int begin;

        Node (int codeword_no = 0, int begin = 0);
    }

    // The underlying buffer.
    Buffer const& m_input;

    // Root node.
    Node* m_root;

    // Codeword array. The first codeword is alway the root, i.e., the empty
    // codeword.
    std::vector<Codeword> m_codewords;

    // An array mapping codeword indices to tree nodes.
    std::vector<Node*> m_nodes;

    // Removes the `i`th codeword from the tree. Not exposed publicly in order
    // to enforce coniguous codeword range. Implicitly called by `extend()`.
    void remove (int i);
};

inline BufferCharSlice EncodeDictNode::edge (int i, char a) {
    Node* node = m_nodes[i];
    Node* child = node->child(a);
    int begin = m_codewords[]
    BufferCharSlice slice(m_codewords[child->codeword_no].begin - )
}

inline Codeword const& EncodeDictTree::codeword (int i) const {
    assert(0 <= i && i < m_codewords.size());
    return m_codewords[i];
}

#endif // ENCODE_DICT_TREE_H
