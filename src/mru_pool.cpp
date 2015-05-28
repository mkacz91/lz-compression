#include "mru_pool.hpp"

MruPool::MruPool (int size) :
    m_next(size > 0 ? -1 : 0)
{
    m_iters.reserve(max(0, size));
}

void MruPool::use (int i) {
    if (this->is_infinite()) {
        // No queue maintained for infinite pools.
        assert(i < m_next);
        return;
    }
    assert(i < m_queue.size());
    m_queue.erase(m_iters[i]);
    m_queue.push_front(i);
    m_iters[i] = m_queue.begin();
}

int MruPool::get () {
    if (this->is_infinite()) {
        // This is an infinite pool.
        return m_next++;
    } else if (m_queue.size() < m_iters.capacity()) {
        // The limit hasn't been reached yet. Return a fresh number.
        int i = m_queue.size();
        m_queue.push_front(i);
        m_iters.push_back(m_queue.begin());
        return i;
    } else {
        // The limit has been reached and and the least recently used number
        // will be utilized.
        int i = m_queue.back();
        m_queue.pop_back();
        m_queue.push_front(i);
        m_iters[i] = m_queue.begin();
        return i;
    }
}

inline bool MruPool::is_infinite () const {
    return m_next != -1;
}
