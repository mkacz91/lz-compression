#ifndef MRU_POOL_HPP
#define MRU_POOL_HPP

#include "prefix.hpp"
#include <vector>
#include <list>

class MruPool {
public:
    explicit MruPool (int size = 0);

    void use (int i);

    int get ();

private:
    typedef std::list<int> IntList;
    typedef std::vector<IntList::iterator> IntListIteratorVector;

    IntList m_queue;
    IntListIteratorVector m_iters;
};

inline MruPool::MruPool (int size) {
    m_iters.reserve(size);
}

#endif // MSU_POOL_HPP
