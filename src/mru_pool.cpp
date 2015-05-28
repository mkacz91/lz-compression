#include "mru_pool.hpp"

void MruPool::use (int i) {
    assert(i < m_queue.size());
    m_queue.erase(m_iters[i]);
    m_queue.push_front(i);
    m_iters[i] = m_queue.begin();
}

int MruPool::get () {
    int i;
    if (m_queue.size() < m_iters.capacity()) {
        i = m_queue.size();
        m_queue.push_front(i);
        m_iters.push_back(m_queue.begin());
    } else {
        i = m_queue.back();
        m_queue.pop_back();
        m_queue.push_front(i);
        m_iters[i] = m_queue.begin();
    }
    return i;
}
