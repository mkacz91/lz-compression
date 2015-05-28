#ifndef MRU_POOL_HPP
#define MRU_POOL_HPP

#include "prefix.hpp"
#include <vector>
#include <list>

// A pool providing numbers from a specified range of type [0, size), using
// the Most Recently Used (abbr. MRU) strategy to choose numbers to invalidate.
class MruPool {
public:
    // Creates a pool with of given size.
    explicit MruPool (int size = 0);

    // Marks `i` as used. It is moved to the front of the MRU queue.
    void use (int i);

    // Provides a number from range [0, size). If the total number of calls to
    // `get()` does not exceed `size`, the returned value is the smallest
    // number not provided yet. If not, the least recently used number, i.e.,
    // the last number on the MRU queue, is returned. The returned number is
    // automatically moved to the front of the MRU queue.
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
