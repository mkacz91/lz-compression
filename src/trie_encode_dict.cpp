#include "trie_encode_dict.hpp"
#include <queue>

TrieEncodeDict::TrieEncodeDict(int limit) :
    m_codeword_pool(limit)
{
    m_nodes.emplace_back(0); // Create root node.
    if (!m_codeword_pool.is_infinite()) {
        // Initialize node pool. Infinite m_codeword_pool ensures `limit > 0`.
        m_nodes.reserve(limit);
        for (int i = 1; i <= limit; ++i)
            m_nodes.emplace_back(i);
    }
    m_node = &m_nodes.front();
}

TrieEncodeDict::~TrieEncodeDict () {
    if (m_codeword_pool.is_infinite()) {
        // This is an unlimited dictionary, so the nodes are allocated on heap.
        std::queue<Node*> nodes;
        for (auto cn : m_nodes[0])
            nodes.push(cn.second);
        while (!nodes.empty()) {
            Node* n = nodes.front();
            nodes.pop();
            for (auto cn : *n)
                nodes.push(cn.second);
            delete n;
        }
    }
}

int TrieEncodeDict::try_char (char a) {
    Node* next_node = m_node->child(a);
    if (next_node != nullptr) {
        m_node = next_node;
        return -1;
    } else {
        // If a codeword is used, all its prefixes are also considered used.
        // The prefixes have to be marked more recent so that the discarded
        // codewords are always leaves.
        for (Node* n = m_node; n->codeword_no() != 0; n = n->parent())
            m_codeword_pool.use(n->codeword_no() - 1);
        m_node->link_child(a, new_node());
        int i = m_node->codeword_no();
        m_node = &m_nodes.front();
        return i;
    }
}

inline EncodeDictNode* TrieEncodeDict::new_node () {
    return m_codeword_pool.is_infinite()
        ? new EncodeDictNode(m_codeword_pool.get() + 1)
        : &m_nodes[m_codeword_pool.get() + 1];
}
