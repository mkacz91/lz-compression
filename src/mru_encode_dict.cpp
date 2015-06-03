#include "mru_encode_dict.hpp"

MruEncodeDict::MruEncodeDict(int limit) :
    MruDict(limit)
{
    // Initialize node pool. First element is always the root.
    m_nodes.reserve(limit + 1);
    m_nodes.emplace_back(0);
    for (int i = 1; i <= limit; ++i)
        m_nodes.emplace_back(i);

    // Search starts from the root.
    m_node = &m_nodes.front();
}

int MruEncodeDict::try_char (char a) {
    int i = m_node->codeword_no();
    Node* next_node = m_node->child(a);
    if (next_node != nullptr) {
        // Still matching. Can possibly be extended.
        m_node = next_node;
        return -1;
    } else {
        // Maximal match found. New node has to be added.
        int j = this->match(i);
        // We don't update the dictionary if the new codeword is too long. This
        // is indicated with `j == 0`
        if (j != 0)
            m_node->link_child(a, &m_nodes[j]);
        m_node = &m_nodes.front();
        return i;
    }
}
