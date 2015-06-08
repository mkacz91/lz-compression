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

    // Number of the corresponding codeword.
    int codeword_no;

    // Constructs a node with given codeword number, which defaults to 0.
    explicit EncodeDictNode (int codeword_no = 0);

    // Returns the parent node. The result is `nullptr` in case of root.
    EncodeDictNode* parent ();

    // Returns the node pointed to by an edge labelled `a`, or `nullptr` if no
    // such edge exists.
    EncodeDictNode* child (char a);

    // Returns the iterator pointing to the first child.
    iterator begin ();

    // Returns the iterator pointing right past the last child.
    iterator end ();

    // Link a child node along an edge with label starting with `a`. Can be
    // called only if this node does not have any child along the same edge. If
    // necessary, `node` is detached from whatever parent it was already linked
    // to.
    void link_child (char a, EncodeDictNode* node);

    // Unlinks the node from parent. Has no effect on root nodes.
    void unlink ();

    // Returns the label of the edge that connects this node with its parent.
    // Invalid for root nodes.
    char link_char () const;

    // Gets the number of children.
    int child_cnt () const;

    // Returns `true` if the node is a root, i.e., has no parent.
    bool is_root () const;

    // Returns `true` if the node is a leaf, i.e., has no children.
    bool is_leaf () const;

private:
    // The parent node. No parent is indicated with `nullptr`.
    EncodeDictNode* m_parent;

    // The hashmap of children nodes. Each entry represents an edge, the key
    // being the edge label.
    ChildMap m_children;

    // Position of this node within the parent's children map. Undefined for
    // root nodes.
    iterator m_children_pos;
};

inline EncodeDictNode::EncodeDictNode (int codeword_no)
    : m_parent(nullptr)
    , codeword_no(codeword_no)
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
        node->m_parent->m_children.erase(node->m_children_pos);
    node->m_parent = this;
    node->m_link_char = a;
    m_children[a] = node;
}

inline void EncodeDictNode::unlink () {
    if (m_parent != nullptr) {
        m_parent->m_children.erase(m_children_pos);
        m_parent = nullptr;
    }
}

inline char EncodeDictNode::link_char () const {
    assert(!is_root())
    return m_children_pos->first;
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

#endif // ENCODE_DICT_NODE_H
