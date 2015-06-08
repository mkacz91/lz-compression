#ifndef LABEL_TREE_NODE_H
#define LABEL_TREE_NODE_H

#include "prefix.h"

#include <unordered_map>

// LabelTreeNode
// =============================================================================
//
// A for nodes in trees with labelled edges and nodes. Parameters `N` and `E`
// stand for the types of node and edge labels, respectively. Type `E` must have // a specialization for `std::hash<>`.
template <typename N, typename E>
class LabelTreeNode {
public:
    typedef std::unordered_map<E, LabelTreeNode*> ChildMap;
    typedef typename ChildMap::iterator iterator;

    N label;

    // Constructs a root node with initialized label. Arguments `args` are
    // passed to `N`'s constructor.
    template <typename... Args>
    explicit LabelTreeNode (Args const&... label_args);

    // Returns the parent node. The result is `nullptr` in case of root.
    LabelTreeNode* parent ();

    // Returns the node pointed to by an edge labelled `e`, or `nullptr` if no
    // such edge exists.
    LabelTreeNode* child (E e);

    // Returns the iterator pointing to the first child.
    iterator begin ();

    // Returns the iterator pointing right past the last child.
    iterator end ();

    // Link a child node along an edge labelled `e`. Can be called only if this
    // node does not have any child along the same edge. If necessary, `node` is
    // detached from whatever parent it was already linked to.
    void link_child (E e, LabelTreeNode* node);

    // Unlinks the node from parent. Has no effect on root nodes.
    void unlink ();

    // Gets the number of children.
    int child_cnt () const;

    // Returns `true` if the node is a root, i.e., has no parent.
    bool is_root () const;

    // Returns `true` if the node is a leaf, i.e., has no children.
    bool is_leaf () const;

private:
    // The parent node. No parent is indicated with `nullptr`.
    LabelTreeNode* m_parent;

    // The hashmap of children nodes. Each entry represents an edge, the key
    // being the edge label.
    ChildMap m_children;

    // Position of this node within the parent's children map. Undefined for
    // root nodes.
    iterator m_children_pos;
};

template <typename N, typename E>
template <typename... Args>
inline LabelTreeNode<N, E>::LabelTreeNode (Args const&... args) :
    m_parent(nullptr),
    label(args...)
{
    /* Do nothing. */
}

template <typename N, typename E>
inline LabelTreeNode<N, E>* LabelTreeNode<N, E>::parent () {
    return m_parent;
}

template <typename N, typename E>
inline LabelTreeNode<N, E>* LabelTreeNode<N, E>::child (E e) {
    auto it = m_children.find(e);
    return it != m_children.end() ? it->second : nullptr;
}

template <typename N, typename E>
inline typename LabelTreeNode<N, E>::iterator LabelTreeNode<N, E>::begin () {
    return m_children.begin();
}

template <typename N, typename E>
inline typename LabelTreeNode<N, E>::iterator LabelTreeNode<N, E>::end () {
    return m_children.end();
}

template <typename N, typename E>
inline void LabelTreeNode<N, E>::link_child (E e, LabelTreeNode* node) {
    auto insert = m_children.insert(make_pair(e, node));
    assert(insert.second);
    node->unlink();
    node->m_parent = this;
    node->m_children_pos = insert.first;
}

template <typename N, typename E>
inline void LabelTreeNode<N, E>::unlink () {
    if (m_parent != nullptr) {
        m_parent->m_children.erase(m_children_pos);
        m_parent = nullptr;
    }
}

template <typename N, typename E>
inline int LabelTreeNode<N, E>::child_cnt () const {
    return m_children.size();
}

template <typename N, typename E>
inline bool LabelTreeNode<N, E>::is_leaf () const {
    return m_children.empty();
}

template <typename N, typename E>
inline bool LabelTreeNode<N, E>::is_root () const {
    return m_parent == nullptr;
}

#endif // LABEL_TREE_NODE_H
