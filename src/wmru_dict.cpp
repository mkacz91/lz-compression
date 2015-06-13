#include "wmru_dict.h"

// WmruPool
// =============================================================================

WmruPool::WmruPool (int limit, bool single_char_codewords) :
    CodewordPool(limit, single_char_codewords),
    m_size(0)
{
    // The 0 index is reserved for the empty codeword, which is not dubject
    // to queueing.
    m_queue_positions.reserve(limit + 1);
    m_queue_positions.push_back(m_queue.end());

    if (single_char_codewords) {
        // Single char codewords are also not subject to queueing.
        m_queue_positions.resize(CHAR_CNT + 1, m_queue.end());
        m_size = CHAR_CNT;
    }
}

int WmruPool::match (int i) {
    assert (0 <= i && i <= m_size);

    // If the codeword is subject to queueing, bring it to the back.
    if (m_queue_positions[i] != m_queue.end()) {
        m_queue.erase(m_queue_positions[i]);
        m_queue.push_back(i);
        m_queue_positions[i] = --m_queue.end();
    }

    if (m_size < limit()) {
        // Use a fresh codeword and put it at the back of the queue.
        m_queue.push_back(++m_size);
        m_queue_positions.push_back(--m_queue.end());
        return m_size;
    } else {
        // Utilize tha least recentyl used codeword and push it to the back of
        // the queue.
        int j = m_queue.front();
        m_queue.pop_front();
        m_queue.push_back(j);
        m_queue_positions[j] = --m_queue.end();
        return j;
    }
}
