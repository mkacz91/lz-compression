#ifndef POOL_DICT_H
#define POOL_DICT_H

#include "prefix.h"
#include <vector>

#include "dict_base.h"

// TODO: documentation
class PoolEncodeDictBase : EncodeDictBase {
public:
    // Constructs a dictionary occupied only with the empty codeword.
    PoolDictBase ();

    ~PoolDictBase ();

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

    // Returns the subsequence of the input buffer that corresponds to the
    // edge from codeword `i` along character `a`.
    BufferCharSlice edge (int i, char a);

    // Returns the `i`th.
    Codeword const& codeword (int i) const;

private:
    // Each node of the tree used to effectively find codeword matches contains
    // information about its active, the corresponding codeword number, and
    // the `begin` and `length` properties defining that codeword.
    //
    // The edges are in turn labelled with whole words, or rather subwords of
    // the input buffer. It suffices to label the edges with the starting
    // positions of their corresponding subwords and thir length is obtained
    // by subtracting the lengths of adjecent nodes.
    struct NodeLabel {
        bool active;
        int codeword_no;
        int begin;
        int length;

        NodeLabel (bool active, int codeword_no, int begin, int length);
    };
    typedef WordTreeNode<NodeLabel, int> Node;

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

template <typename Pool>
class PoolDictBase : public DictBase {
public:
    explicit PoolDictBase (int limit);

    // Implements `DictBase::make_permanent(int)`.
    virtual void make_permanent (int i);

private:
    Pool m_pool;
};

template <typename Pool>
PoolDictBase<Pool>::PoolDictBase (int limit) :
    DictBase(limit),
    m_pool(limit)
{
    /* Do nothing. */
}

template <typename Pool>
PoolDictBase<Pool>::make_permanent (int i) {
    m_pool.make_permanent(i);
}

template <typename Pool>
class PoolEncodeDict : public PoolDictBase<Pool> {
public:
    // Implements `EncodeDictBase::try_char(char)`.
    virtual Match try_char (char a);

    // Implements `EncodeDictBase::fail_char()`.
    virtual Match fail_chat ();

private:
    PoolDictTree m_tree;
    Buffer m_input;
    BufferCharReader m_reader;
    BufferCharSlice m_edge_slice;
    Match m_match;
    int m_extending_char_pos;
    PoolDictTree::Edge m_edge;
    int m_edge_pos;
};

template <typename Pool>
Match PoolEncodeDict<Pool>::try_char () {
    char a = m_reader.get();
    bool matches = false;
    if (m_edge_pos == 0) {
        m_match.extending_char = a;
        m_extending_char_pos = m_reader.
        m_edge = m_tree.edge(m_next_codeword, m_reader.pos());
        if (m_edge.dest_codeword_no != 0) {
            m_edge_slice = BufferCharSlice(m_input, edge.begin, edge.length);
            matches = true;
        }
    } else {
        matches = (m_edge_slice[m_edge_pos] == a);
    }

    if (matches) {
        ++m_edge_pos;
        if (m_edge_pos == m_edge_slice.length()) {
            m_match.codeword_no = m_edge.dest_codeword_no;
            m_match.length += m_edge.length;
            m_edge_pos = 0;
        }
        return Match();
    } else {
        int new_codeword_no = m_pool.get(m_match);
        m_tree.extend(m_match.codewodr_no, m_reader.pos(),
        return m_match;
    }
}

inline BufferCharSlice WordTreeNode::edge (int i, char a) {
    Node* node = m_nodes[i];
    Node* child = node->child(a);
    int begin = m_codewords[]
    BufferCharSlice slice(m_codewords[child->codeword_no].begin - )
}

inline Codeword const& PoolDictBase::codeword (int i) const {
    assert(0 <= i && i < m_codewords.size());
    return m_codewords[i];
}

#endif // POOL_DICT_H
