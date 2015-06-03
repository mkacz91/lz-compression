#include "mru_pool.hpp"

MruPool::MruPool (int size) :
    m_next(size > 0 ? -1 : 0)
{
    m_iters.reserve(max(0, size));
}

void MruPool::use (int i) {
    // Calling this for an infinite pool is a waste of time.
    assert(!this->is_infinite());
    assert(i < m_queue.size());

    m_queue.erase(m_iters[i]);
    m_queue.push_front(i);
    m_iters[i] = m_queue.begin();
}

void MruPool::use_before (int j, int i) {
    // Calling this for an infinite pool is a waste of time.
    assert(!this->is_infinite());
    assert(i < m_queue.size());
    assert(j < m_queue.size());

    m_queue.erase(m_iters[i]);
    auto it = m_iters[j];
    m_iters[i] = m_queue.insert(++it, i);
}

int MruPool::get () {
    if (this->is_infinite()) {
        // This is an infinite pool.
        return m_next++;
    } else if (m_queue.size() < m_iters.capacity()) {
        // The limit hasn't been reached yet. Return a fresh number.
        int i = m_queue.size();
        m_queue.push_back(i);
        m_iters.push_back(--m_queue.end());
        return i;
    } else {
        // The limit has been reached and and the least recently used number
        // will be utilized.
        return m_queue.back();
    }
}
