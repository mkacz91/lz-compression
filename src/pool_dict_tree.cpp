#include "pool_dict_tree.h"

#include <queue>

// PoolDictTree
// =============================================================================

PoolDictTree::PoolDictTree (Buffer const& input) :
    m_input(input),
    m_root(new Node(true, 0, 0, 0)),
    m_nodes(1, m_root)
{
    BufferCharWriter alphabet_writer(m_alphabet);
    alphabet_writer.put('\0'); // Dummy char to make the indexing 1-based.
    for (int a = 0; a < CHAR_CNT; ++a)
        alphabet_writer.put(a);
}

PoolDictTree::~PoolDictTree () {
    // All nodes are allocated on heap.
    std::queue<Node*> queue;
    queue.push(m_root);
    while (!queue.empty()) {
        Node* node = queue.front();
        queue.pop();
        for (auto kv : *node)
            queue.push(kv.second);
        delete node;
    }
}

void PoolDictTree::extend (int i, int begin, int j) {
    assert(0 <= i && i <  m_nodes.size());
    assert(0 <  j && j <= m_nodes.size());

    Node* to_remove = nullptr;
    if (j == m_nodes.size()) {
        m_nodes.push_back(nullptr);
    } else {
        to_remove = m_nodes[j];
    }

    // The node which will become the parent of the new node.
    Node* const upper = m_nodes[i];
    assert(upper != nullptr);
    assert(upper->tag.active);

    // Length of the new codeword.
    int const length = upper->tag.length + 1;

    // This is rather wacky way of obtaining the linking character. Fixing this
    // would involve either passing it as an argument to this method or allowing
    // random access to `Buffer`, which would break encapsulation. Maybe
    // someday...
    char a = slice(begin, length)[length - 1];

    // A node that already stems from the extended one along the same edge.
    // It may become the new node itself or its child, depending on the
    // arrangement.
    Node* lower = upper->child(a);

    if (lower == nullptr) {
        // There is no node hanging from `upper` along `a`, so we simply
        // attach one and we're done.
        Node* fresh = new Node(true, j, begin, length);
        upper->link_child(a, fresh);
        m_nodes[j] = fresh;
    } else if (lower->tag.length > length){
        // Node `lower` is an even greater extension of `upper`. The edge
        // between `upper` and `lower` has to be split.
        Node* fresh = new Node(true, j, begin, length);
        char b = slice(lower->tag.begin, length + 1)[length];
        // This order of relinking is important, because first `lower` has to
        // be detached from `upper`.
        fresh->link_child(b, lower);
        upper->link_child(a, fresh);
        m_nodes[j] = fresh;
    } else {
        // Node `lower` is in the exact same place where we want to put the
        // new node. In a well behaved dictionary this is possible only if
        // `lower` is an inactive node, so we can just use it.
        assert(!lower->tag.active);
        lower->tag = Tag(true, j, begin, length);
        m_nodes[j] = lower;
    }

    if (to_remove != nullptr)
        remove(to_remove);
}

PoolDictTree::Edge PoolDictTree::edge (Node const* node, char a) const {
    assert(0 <= node->tag.codeword_no);
    assert(node->tag.codeword_no < m_nodes.size());
    assert(m_nodes[node->tag.codeword_no] == node || !node->tag.active);

    Node* child = const_cast<Node*>(node)->child(a);
    if (child == nullptr) {
        return edge_to_root();
    } else {
        int begin = child->tag.begin + node->tag.length;
        int length = child->tag.length - node->tag.length;
        return Edge(child, slice(begin, length));
    }
}

void PoolDictTree::remove (Node* node) {
    assert(node != nullptr);
    assert(node->tag.active);
    assert(!node->is_root());

    if (node->is_leaf()) {
        Node* parent = node->parent();
        node->unlink();
        delete node;
        assert(parent->tag.active || parent->child_cnt() > 0);
        // Removing a leaf may yield an inactive node that has only one child.
        // In such case we have to shortcut it to its grandparent.
        if (!parent->tag.active && parent->child_cnt() == 1) {
            assert(!parent->is_root());
            char a = parent->link_char();
            Node* grandparent = parent->parent();
            parent->unlink();
            grandparent->link_child(a, parent->begin()->second);
            delete parent;
        }
    } else if (node->child_cnt() == 1) {
        // This node has only one child. That child has to be shortcut to its
        // grandparent.
        char a = node->link_char();
        Node* parent = node->parent();
        Node* child = node->begin()->second;
        node->unlink();
        parent->link_child(a, child);
        delete node;
    } else {
        // We deal with a forking node. It suffices to just mark it as inactive.
        node->tag.active = false;
    }
}

inline BufferCharSlice PoolDictTree::slice (int begin, int length) const {
    return begin < 0
        ? BufferCharSlice(m_alphabet, -begin, length)
        : BufferCharSlice(m_input, begin, length);
}
