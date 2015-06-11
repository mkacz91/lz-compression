#ifndef POOL_DICT_TREE_H
#define POOL_DICT_TREE_H

// TODO: documentation
class PoolDictTree : EncodeDictBase {
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

#endif // POOL_DICT_TREE_H
