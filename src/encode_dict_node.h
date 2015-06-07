#ifndef ENCODE_DICT_NODE_H
#define ENCODE_DICT_NODE_H

#include "prefix.h"

#include <unordered_map>

// EncodeDictNode
// =============================================================================
//
// A word-tree node for use in LZ78/LZW dictionary implementation during
// encoding.
//
// Although different implementations of dictionaries are tried, the
// node structure is mostly the same.
class EncodeDictNode {
public:
    typedef std::unordered_map<char, EncodeDictNode*> ChildMap;
    typedef ChildMap::iterator iterator;

    // Constructs a node with given codeword number. It is impossible to
    // alter the codeword number afterwards.
    explicit EncodeDictNode (int codeword_no);

    // Returns the parent node. The result is `nullptr` in case of root.
    EncodeDictNode* parent ();

    // Returns the node pointed to by an edge with label starting with `a`.
    // The result is `nullptr` if there is no such edge.
    EncodeDictNode* child (char a);

    // Returns the iterator pointing to the first child.
    iterator begin ();

    // Returns the iterator pointing right past the last child.
    iterator end ();

    // Link a child node along an edge with label starting with `a`. Detaching
    // a child is only possible through linking it to another parent.
    void link_child (char a, EncodeDictNode* node);

    // Gets the number of children.
    int child_cnt () const;

    // Returns `true` if the node is a root, i.e., has no parent.
    bool is_root () const;

    // Returns `true` if the node is a leaf, i.e., has no children.
    bool is_leaf () const;

    // Returns the number of the corresponding codeword.
    int codeword_no () const;

private:

    // The parent node. No parent is indicated with `nullptr`.
    EncodeDictNode* m_parent;

    // The letter along which the node is linked to its parent. Undefined for
    // unlinked nodes.
    char m_link_char;

    // The hashmap of children nodes. Each entry represents an edge, the key
    // being the first letter of the edge label.
    ChildMap m_children;

    // The number of the corresponding codeword.
    int m_codeword_no;
};

inline EncodeDictNode::EncodeDictNode (int codeword_no)
    : m_parent(nullptr)
    , m_codeword_no(codeword_no)
{
    assert(codeword_no >= 0);
}

inline EncodeDictNode* EncodeDictNode::parent () {
    return m_parent;
}

inline EncodeDictNode* EncodeDictNode::child (char a) {
    ChildMap::iterator it = m_children.find(a);
    return it != m_children.end() ? it->second : nullptr;
}

inline EncodeDictNode::iterator EncodeDictNode::begin () {
    return m_children.begin();
}

inline EncodeDictNode::iterator EncodeDictNode::end () {
    return m_children.end();
}

inline void EncodeDictNode::link_child (char a, EncodeDictNode* node) {
    assert(m_children.count(a) == 0);
    if (node->m_parent != nullptr)
        node->m_parent->m_children.erase(node->m_link_char);
    node->m_parent = this;
    node->m_link_char = a;
    m_children[a] = node;
}

inline int EncodeDictNode::child_cnt () const {
    return m_children.size();
}

inline bool EncodeDictNode::is_leaf () const {
    return m_children.empty();
}

inline bool EncodeDictNode::is_root () const {
    return m_parent == nullptr;
}

inline int EncodeDictNode::codeword_no () const {
    return m_codeword_no;
}

#endif // ENCODE_DICT_NODE_H