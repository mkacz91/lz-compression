#include "smru_dict.h"

// SmruDictBase
// =============================================================================

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
    // A permanent codeword is recognized as being used after all other, namely
    // `m_queue_positions[i] == m_queue.end()`. To ensure further integrity,
    // all prefixes of `i` must also be marked permanent.
    for (int j = i; m_queue_positions[j] != m_queue.end(); j = m_parents[j]) {
        m_queue.erase(m_queue_positions[j]);
        m_queue_positions[j] = m_queue.end();
    }
}

int SmruDictBase::match (int i) {
    assert(0 <= i && i <= size());

    // If a codeword is used, its prefixes are considered used more recently.
    // The traversal stops when a permanent codeword is reached.
    for (int j = i; m_queue_positions[j] != m_queue.end(); j = m_parents[j]) {
        m_queue.erase(m_queue_positions[j]);
        m_queue.push_back(j);
        m_queue_positions[j] = --m_queue.end();
    }

    // Try to make a fresh codeword.
    int j = fresh_codeword_no();
    if (j != 0) {
        // The values below will be overwritten in the upcoming instructions
        // but that's ok. It removes some redundancy in the follow up code.
        m_queue.push_front(j);
        m_parents.push_back(i);
        m_queue_positions.push_back(m_queue.begin());
    } else {
        // The limit has been reached. Utilize the least recently used
        // codeword.
        j = m_queue.front();
    }

    // Don't permit codewords longer than the limit. Such situation is
    // detected when the codeword to be discarded is the longest match
    // itself.
    if (i != j) {
        m_queue.erase(m_queue_positions[j]);
        if (i == 0) {
            m_queue.push_back(j);
            m_queue_positions[j] = --m_queue.end();
        } else {
            m_queue_positions[j] = m_queue.insert(m_queue_positions[i], j);
        }
        m_parents[j] = i;
        return j;
    } else {
        return 0;
    }
}

// SmruEncodeDict
// =============================================================================

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
    m_match_length = 0;
}

// SmruDecodeDict
// =============================================================================

SmruDecodeDict::SmruDecodeDict (int limit) :
    SmruDictBase(limit),
    m_codewords(limit + 1, Codeword(0, 0))
{
    /* Do nothing */
}

Match SmruEncodeDict::try_char (char a) {
    int i = m_node->codeword_no;
    Node* next_node = m_node->child(a);
    if (next_node != nullptr) {
        // Still matching. This is not necessarily a maximal match.
        m_node = next_node;
        ++m_match_length;
        return Match();
    } else {
        // Maximal match found. New node has to be added.
        int j = this->match(i);
        // Don't update the dictionary if the new codeword is too long. This
        // is indicated with `j == 0`.
        if (j != 0)
            m_node->link_child(a, &m_nodes[j]);
        int length = m_match_length;
        // New search starts at root.
        m_node = &m_nodes.front();
        m_match_length = 0;
        return Match(i, length, a);
    }
}

Match SmruEncodeDict::fail_char () {
    int i = m_node->codeword_no;
    // New search starts at root.
    m_node = &m_nodes.front();
    return Match(i, m_match_length, '\0'); // The extending char is irrelevant.
}
