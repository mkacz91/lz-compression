#include "smru_dict.h"

// SmruDict
// =============================================================================

SmruDict::SmruDict (int limit, bool single_char_codewords) :
    Dict(limit, single_char_codewords),
    m_size(0)
{
    assert(limit > 0);
    assert(!single_char_codewords || limit >= CHAR_CNT);

    // The 0 index is reserved for the empty codeword.
    m_parents.reserve(limit + 1);
    m_queue_positions.reserve(limit + 1);
    m_parents.push_back(0);
    // Root is not subject to queueing.
    m_queue_positions.push_back(m_queue.end());
    
    if (single_char_codewords) {
        m_parents.resize(CHAR_CNT + 1, 0);
        // Note that single char words are also not subject to queueing.
        m_queue_positions.resize(CHAR_CNT + 1, m_queue.end());
        m_size = CHAR_CNT;
    }
}

int SmruDict::match (int i) {
    assert(0 <= i && i <= m_size);

    // If a codeword is used, its prefixes are considered used more recently.
    // The traversal stops when a permanent codeword is reached.
    for (int j = i; m_queue_positions[j] != m_queue.end(); j = m_parents[j]) {
        m_queue.erase(m_queue_positions[j]);
        m_queue.push_back(j);
        m_queue_positions[j] = --m_queue.end();
    }

    int j;
    if (m_size < m_limit) {
        // We can use a fresh codeword.
        j = ++m_size;
        m_queue.push_front(j);
        m_parents.push_back(i);
        m_queue_positions.push_back(m_queue.begin());
    } else {
        // Utilize the least recently used codeword.
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

SmruEncodeDict::SmruEncodeDict(
    Buffer const& input,
    int limit,
    bool single_char_codewords
) :
    SmruDict(limit, single_char_codewords),
    EncodeDict(input)
{
    // Initialize node pool. First element is always the root.
    m_nodes.reserve(limit + 1);
    m_nodes.emplace_back(0);
    for (int i = 1; i <= limit; ++i)
        m_nodes.emplace_back(i);

    // Create the single letter nodes if necessary
    if (single_char_codewords) {
        for (int a = 0; a < CHAR_CNT; ++a)
            m_nodes.front().link_child(a, &m_nodes[a + 1]);
    }
    
    // Search starts at root.
    m_node = &m_nodes.front();
    m_match_length = 0;
}

Match SmruEncodeDict::try_char () {
    char a = this->get_char();
    Node* next_node = m_node->child(a);
    if (next_node != nullptr) {
        // Still matching. This is not necessarily a maximal match.
        m_node = next_node;
        ++m_match_length;
        return Match();
    } else {
        // Maximal match found. New node has to be added.
        int i = m_node->tag;
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
    int i = m_node->tag;
    // New search starts at root.
    m_node = &m_nodes.front();
    return Match(i, m_match_length, '\0'); // The extending char is irrelevant.
}

// SmruDecodeDict
// =============================================================================

SmruDecodeDict::SmruDecodeDict (int limit, bool single_char_codewords) :
    SmruDict(limit, single_char_codewords),
    m_codewords(limit + 1, Codeword(0, 0))
{
    if (single_char_codewords) {
        for (int a = 0; a < CHAR_CNT; ++a)
            m_codewords[a + 1].length = 1;
    }
}
