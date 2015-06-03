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
        // Still matching. Can possibly be extended.
        m_node = next_node;
        return -1;
    } else {
        // Maximal match found. New node has to be added. In an unlimited
        // dictionary, we just allocate on the heap, but otherwise, we tekae one
        // from the pool, possibly sacrificing an existing node.
        if (m_codeword_pool.is_infinite()) {
            m_node->link_child(a, new Node(m_codeword_pool.get() + 1));
        } else {
            // If a codeword is used, all its prefixes are also considered used.
            // The prefixes have to be marked more recent so that the discarded
            // codewords are always leaves.
            for (Node* n = m_node; n->codeword_no() != 0; n = n->parent())
                m_codeword_pool.use(n->codeword_no() - 1);
            int i = m_codeword_pool.get();
            int j = m_node->codeword_no() - 1;
            // If the new codeword is longer than limit, reject. Such situation // is detected when attempting to simultaneously discard and link to
            // the same node, meaning that the trie is in fact a single path.
            if (i != j) {
                if (j < 0) // append to root
                    m_codeword_pool.use(i);
                else
                    m_codeword_pool.use_before(j, i);
                m_node->link_child(a, &m_nodes[i + 1]);
            }
        }
        int cw = m_node->codeword_no();
        m_node = &m_nodes.front();
        return cw;
    }
}
