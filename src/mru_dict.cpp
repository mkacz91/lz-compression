#include "mru_dict.hpp"

MruDict::MruDict (int limit) :
    m_limit(limit)
{
    assert(limit > 0);
    // The arrays are initiated with dummy entries because we want to have
    // 1-based indexing.
    m_parents.reserve(limit + 1);
    m_queue_positions.reserve(limit + 1);
    m_parents.push_back(0);
    m_queue_positions.push_back(m_queue.end());
}

int MruDict::match (int i) {
    assert(0 <= i && i <= m_queue.size());

    // If a codeword is used, its prefixes are considered used later. This
    // ensures that only the leaf codewords are discarded.
    for (int j = i; j != 0; j = m_parents[j]) {
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
