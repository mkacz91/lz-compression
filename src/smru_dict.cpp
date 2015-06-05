#include "smru_dict.h"

SmruDictBase::SmruDictBase (int limit) :
    DictBase(limit)
{
    assert(limit > 0);
    // The arrays are initiated with dummy entries because we want to have
    // 1-based indexing.
    m_parents.reserve(limit + 1);
    m_queue_positions.reserve(limit + 1);
    m_parents.push_back(0);
    // Note that this makes the root behave like a permanent codeword.
    m_queue_positions.push_back(m_queue.end());
}

void SmruDictBase::make_permanent (int i) {
    // A permanent codeword is recognized with
    // `m_queue_positions[i] == m_queue.end()`. To ensure further integrity,
    // all prefixes of `i` must also be marked permanent.
    for (int j = i; m_queue_positions[j] != m_queue.end(); j = m_parents[j]) {
        m_queue.erase(m_queue_positions[j]);
        m_queue_positions[j] = m_queue.end();
    }
}

int SmruDictBase::match (int i) {
    assert(0 <= i && i <= m_queue.size());

    // If a codeword is used, its prefixes are considered used later. The
    // traversal stops when a permanent codeword is reached, which is indicated
    // by `m_queue_positions[j] == m_queue.end()`.
    for (int j = i; m_queue_positions[j] != m_queue.end(); j = m_parents[j]) {
        m_queue.erase(m_queue_positions[j]);
        m_queue.push_front(j);
        m_queue_positions[j] = m_queue.begin();
    }

    // If the limit has not been reached, make a fresh codeword. Otherwise,
    // utilize the least recently used codeword.
    int j;
    if (m_queue.size() < m_limit) {
        j = m_queue.size() + 1;
        // The values below will be overwritten in the upcoming instructions
        // but that's ok. It removes some redundancy in the follow up code.
        m_queue.push_back(j);
        m_parents.push_back(i);
        m_queue_positions.push_back(--m_queue.end());
    } else {
        j = m_queue.back();
    }

    // Don't permit codewords longer than the limit. Such situation is
    // detected when the codeword to be discarded is the longest match
    // itself.
    if (i != j) {
        m_queue.erase(m_queue_positions[j]);
        if (i == 0) {
            m_queue.push_front(j);
            m_queue_positions[j] = m_queue.begin();
        } else {
            std::list<int>::iterator pos = m_queue_positions[i];
            m_queue_positions[j] = m_queue.insert(++pos, j);
        }
        m_parents[j] = i;
        return j;
    } else {
        return 0;
    }
}

SmruEncodeDict::SmruEncodeDict(int limit) :
    SmruDictBase(limit)
{
    // Initialize node pool. First element is always the root.
    m_nodes.reserve(limit + 1);
    m_nodes.emplace_back(0);
    for (int i = 1; i <= limit; ++i)
        m_nodes.emplace_back(i);

    // Search starts at root.
    m_node = &m_nodes.front();
}

SmruDecodeDict::SmruDecodeDict (int limit) :
    SmruDictBase(limit),
    m_codewords(limit + 1, Codeword(0, 0))
{
    // Do nothing.
}

int SmruEncodeDict::try_char (char a) {
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
        // New search starts at root.
        m_node = &m_nodes.front();
        return i;
    }
}
