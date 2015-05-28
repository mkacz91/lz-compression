#ifndef MRU_POOL_HPP
#define MRU_POOL_HPP

#include "prefix.hpp"
#include <vector>
#include <list>

// A pool providing numbers from a specified range of type [0, size), using
// the Most Recently Used (abbr. MRU) strategy to choose numbers to invalidate.
// If `size <= 0`, the pool is infinite.
class MruPool {
public:
    // Creates a pool with of given size. Passing `size <= 0` results in an
    // infinite pool.
    explicit MruPool (int size = 0);

    // Marks `i` as used. It is moved to the front of the MRU queue. Has no
    // effect on infinite pools.
    void use (int i);

    // Provides a number from range [0, size). If the total number of calls to
    // `get()` does not exceed `size`, the returned value is the smallest
    // number not provided yet. If not, the least recently used number, i.e.,
    // the last number on the MRU queue, is returned. The returned number is
    // automatically moved to the front of the MRU queue.
    //
    // If the pool is infinite, the results is simply consecitive integers
    // starting at 0.
    int get ();

private:
    typedef std::list<int> IntList;
    typedef std::vector<IntList::iterator> IntListIteratorVector;

    // The MRU queue
    IntList m_queue;

    // Iterators pointing to positions of numbers within the MRU queue.
    IntListIteratorVector m_iters;

    // The number to be returned during next call to `get()` of an infinite
    // pool. Set to -1 in finite pools.
    int m_next;

    // Returns `true` if this is an infinite pool.
    bool is_infinite () const;
};

#endif // MSU_POOL_HPP
