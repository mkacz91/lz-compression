#ifndef WORD_TREE_NODE_H
#define WORD_TREE_NODE_H

#include "prefix.h"

#include <unordered_map>

// WordTreeNode
// =============================================================================
//
// A node of a word tree with vertices holding information of type `T`.
//
// TODO
template <typename T>
class WordTreeNode {
public:
    typedef std::unordered_map<char, WordTreeNode*> ChildMap;
    typedef typename ChildMap::iterator iterator;

    T tag;

    // Constructs a root node with initialized tag. Arguments `args` are
    // passed to `tag`'s condtructor.
    template <typename... Args>
    explicit WordTreeNode (Args const&... label_args);

    // Returns the parent node. The result is `nullptr` in case of root.
    WordTreeNode* parent ();

    // Returns the node pointed to by an edge labelled `a`, or `nullptr` if no
    // such edge exists.
    WordTreeNode* child (char a);

    // Returns the iterator pointing to the first child.
    iterator begin ();

    // Returns the iterator pointing right past the last child.
    iterator end ();

    // Link a child node along an edge labelled `a`. Can be called only if this
    // node does not have any child along the same edge. If necessary, `node` is
    // detached from whatever parent it was already linked to.
    void link_child (char a, WordTreeNode* node);

    // Unlinks the node from parent. Has no effect on root nodes.
    void unlink ();

    // Returns the label of edge pointing to parent. Invalid for root nodes.
    char link_char () const;

    // Gets the number of children.
    int child_cnt () const;

    // Returns `true` if the node is a root, i.e., has no parent.
    bool is_root () const;

    // Returns `true` if the node is a leaf, i.e., has no children.
    bool is_leaf () const;

private:
    // The parent node. No parent is indicated with `nullptr`.
    WordTreeNode* m_parent;

    // The hashmap of children nodes. Each entry represents an edge, the key
    // being the edge label.
    ChildMap m_children;

    // Position of this node within the parent's children map. Undefined for
    // root nodes.
    iterator m_children_pos;
};

template <typename T>
template <typename... Args>
inline WordTreeNode<T>::WordTreeNode (Args const&... args) :
    tag(args...),
    m_parent(nullptr)
{
    /* Do nothing. */
}

template <typename T>
inline WordTreeNode<T>* WordTreeNode<T>::parent () {
    return m_parent;
}

template <typename T>
inline WordTreeNode<T>* WordTreeNode<T>::child (char a) {
    auto it = m_children.find(a);
    return it != m_children.end() ? it->second : nullptr;
}

template <typename T>
inline typename WordTreeNode<T>::iterator WordTreeNode<T>::begin () {
    return m_children.begin();
}

template <typename T>
inline typename WordTreeNode<T>::iterator WordTreeNode<T>::end () {
    return m_children.end();
}

template <typename T>
inline void WordTreeNode<T>::link_child (char a, WordTreeNode* node) {
    auto insert = m_children.insert(make_pair(a, node));
    assert(insert.second);
    node->unlink();
    node->m_parent = this;
    node->m_children_pos = insert.first;
}

template <typename T>
inline void WordTreeNode<T>::unlink () {
    if (m_parent != nullptr) {
        m_parent->m_children.erase(m_children_pos);
        m_parent = nullptr;
    }
}

template <typename T>
inline char WordTreeNode<T>::link_char () const {
    assert(m_parent != nullptr);
    return m_children_pos->first;
}

template <typename T>
inline int WordTreeNode<T>::child_cnt () const {
    return m_children.size();
}

template <typename T>
inline bool WordTreeNode<T>::is_leaf () const {
    return m_children.empty();
}

template <typename T>
inline bool WordTreeNode<T>::is_root () const {
    return m_parent == nullptr;
}

#endif // WORD_TREE_NODE_H
