#ifndef DICTIONARY_NODE_HPP
#define DICTIONARY_NODE_HPP

#include "prefix.hpp"

#include <unordered_map>

// A word-tree node for use in LZ78/LZW dictionary implementation. Although
// different implementations of dictionaries are tried, the node structure
// is mostly the same.
class DictionaryNode {
public:
    typedef std::unordered_map<char, DictionaryNode*> ChildMap;
    typedef ChildMap::iterator iterator;

    // Constructs a node with given codeword number. It is impossible to
    // alter the codeword number afterwards. Negative numbers are treated as 0.
    explicit DictionaryNode (int codeword_no);

    // Returns the parent node. The result is `nullptr` in case of root.
    DictionaryNode* parent ();

    // Returns the node pointed to by an edge with label starting with `a`.
    // The result is `nullptr` if there is no such edge.
    DictionaryNode* child (char a);

    // Returns the iterator pointing to the first child.
    iterator begin ();

    // Returns the iterator pointing right past the last child.
    iterator end ();

    // Link a child node along an edge with label starting with `a`. Detaching
    // a child is only possible through linking it to another parent.
    void link_child (char a, DictionaryNode* node);

    // Gets the number of children.
    int child_cnt () const;

    // Returns `true` if the node is a root, i.e., has no parent.
    bool is_root () const;

    // Returns `true` if the node is a leaf, i.e., has no children.
    bool is_leaf () const;

    // Returns the number of the corresponding codeword. Not valid for inactive
    // nodes.
    int codeword_no () const;

    // Returns `true` if the node is active. This property is not necessarily
    // used by all dictionary. implementations.
    bool is_active () const;

    // Makes the node inactive. An inactive node cannot be returned to the
    // active state. Shouldn't be called on a root node.
    void deactivate ();

private:

    // The parent node. No parent is indicated with `nullptr`.
    DictionaryNode* m_parent;

    // The letter along which the node is linked to its parent. Undefined for
    // unlinked nodes.
    char m_link_char;

    // The hashmap of children nodes. Each entry represents an edge, the key
    // being the first letter of the edge label.
    ChildMap m_children;

    // The number of the corresponding codeword.
    int m_codeword_no;
};

inline DictionaryNode::DictionaryNode (int codeword_no)
    : m_parent(nullptr)
    , m_codeword_no(max(0, codeword_no))
{
    // Do nothing
}

inline DictionaryNode* DictionaryNode::parent () {
    return m_parent;
}

inline DictionaryNode* DictionaryNode::child (char a) {
    ChildMap::iterator it = m_children.find(a);
    return it != m_children.end() ? it->second : nullptr;
}

inline DictionaryNode::iterator DictionaryNode::begin () {
    return m_children.begin();
}

inline DictionaryNode::iterator DictionaryNode::end () {
    return m_children.end();
}

inline void DictionaryNode::link_child (char a, DictionaryNode* node) {
    assert(m_children.count(a) == 0);
    if (node->m_parent != nullptr)
        node->m_parent->m_children.erase(node->m_link_char);
    node->m_parent = this;
    node->m_link_char = a;
    m_children[a] = node;
}

inline int DictionaryNode::child_cnt () const {
    return m_children.size();
}

inline bool DictionaryNode::is_leaf () const {
    return m_children.empty();
}

inline bool DictionaryNode::is_root () const {
    return m_parent == nullptr;
}

inline int DictionaryNode::codeword_no () const {
    assert(this->is_active());
    return m_codeword_no;
}

inline bool DictionaryNode::is_active () const {
    return m_codeword_no != -1;
}

inline void DictionaryNode::deactivate () {
    assert(!this->is_root());
    m_codeword_no = -1;
}

#endif // DICTIONARY_NODE_HPP
