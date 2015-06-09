#include "encode_dict_tree.h"

#include <queue>

// EncodeDictTree
// =============================================================================

EncodeDictTree::EncodeDictTree (Buffer const& input) :
    m_input(input),
    m_root(new Node),
    m_codewords(1, Codeword(0, 0)),
    m_nodes(1, m_root)
{
    /* Do nothing */
}

EncodeDictTree::~EncodeDictTree () {
    // All nodes are allocated on heap.
    std::queue<Node*> queue;
    queue.push(m_root);
    while (!queue.empty()) {
        Node* node = queue.front();
        queue.pop();
        for (Node* child : node)
            queue.push(child);
        delete node;
    }
}

void EncodeDictTree::extend (int i, int begin, int j) {
    assert(0 <= i && i <  m_codewords.size());
    assert(0 <  j && j <= m_codewords.size());

    // If necessary, create new codeword slot.
    if (j == m_codewords.size()) {
        m_codewords.emplace_back(0, 0);
        m_nodes.push_back(nullptr);
    }
    assert(m_nodes[j] == nullptr);

    // Length of the new codeword
    int const length = m_codewords[i].length + 1;

    // This is rather wacky way of obtaining the linking character. Fixing this
    // would involve either passing it as an argument to this method or allowing
    // random access to `Buffer`, which would break encapsulation. Maybe
    // someday...
    char a = BufferCharSlice(m_input, begin, length)[length - 1];

    // The node which will become the parent of the new node.
    Node* const upper = m_nodes[i];
    assert(upper != nullptr);
    assert(upper->active);

    // A node that already stems from the extended one along the same edge.
    // It may become the new node itself or its child, depending on the
    // arrangement.
    Node* lower = upper.child(a);

    if (lower == nullptr) {
        // There is no node hanging from `upper` along `a`, so we simply
        // attach one and we're done.
        Node* fresh = new Node(j, begin);
        upper->link_child(a, fresh);
        m_nodes[j] = fresh;
    } else if (m_codewords[lower->codeword_no].length > length){
        // Node `lower` is an even greater extension of `upper`. The edge
        // between `upper` and `lower` has to be split.
        Node* fresh = new Node(j);
        char b = BufferCharSlice(m_input, fresh->begin, length + 1)][length];
        // This order of relinking is important, because first `lower` has to
        // be detached from `upper`.
        fresh->link_child(b, lower);
        upper->link_child(a, fresh);
    } else {
        // Node `lower` is in the exact same place where we want to put the
        // new node. In a well behaved dictionary this is possible only if
        // `lower` is an inactive node, so we can just use it.
        assert(!lower->active);
        lower->codeword_no = j;
        lower->begin = begin;
        lower->active = true;
        m_codewords[j] = lower;
    }
}

void EncodeDictTree::remove (int i) {
    assert(0 < i && i < m_codewords.size());
    Node* node = m_nodes[i];
    assert(node != nullptr);
    assert(node->active);
    assert(!node->is_root());

    if (node->is_leaf()) {
        Node* parent = node->parent;
        node->unlink();
        delete node;
        assert(parent->active || parent->child_cnt() > 0);
        // Removing a leaf may yield an inactive node that has only one child.
        // In such case we have to shortcut it to its grandparent.
        if (!parent->active && parent->child_cnt() == 1) {
            assert(parent->is_root());
            char a = parent->link_char();
            Node* grandparent = parent->parent();
            parent->unlink();
            grandparent->link_child(a, parent->begin()->second);
            delete parent;
        }
    } else if (node->child_cnt() == 1) {
        // This node has only one child. This child has to be shortcut to its
        // grandparent.
        char a = node->link_char();
        Node* parent = node->parent();
        Node* child = node->begin()->second;
        node->unlink();
        parent->link_child(a, child);
        delete node;
    } else {
        // We deal with a forking node. It suffices to just mark it as inactive.
        node->active = false;
    }
}

// EncodeDictTree::Node
// =============================================================================

inline EncodeDictTree::Node::Node (int codeword_no, int begin) :
    LabelTreeNode(codeword_no),
    active(true),
    begin(begin),
{
    assert(begin >= 0);
}
