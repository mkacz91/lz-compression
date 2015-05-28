#include "trie_dictionary.hpp"

TrieDictionary::TrieDictionary(int limit) :
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

int TrieDictionary::try_char (char a) {
    Node* next_node = m_node->child(a);
    if (next_node != nullptr) {
        m_node = next_node;
        return -1;
    } else {
        m_node->link_child(a, new_node());
        int i = m_node->codeword_no();
        m_node = &m_nodes.front();
        return i;
    }
}

inline DictionaryNode* TrieDictionary::new_node () {
    // Note that m_codeword_pool is 0-based. That's the reason for `+ 1`.
    return m_codeword_pool.is_infinite()
        ? new DictionaryNode(m_codeword_pool.get() + 1)
        : &m_nodes[m_codeword_pool.get() + 1];
}
