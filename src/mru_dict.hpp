#ifndef MRU_DICT_HPP
#define MRU_DICT_HPP

#include "prefix.hpp"
#include <vector>
#include <list>

class MruDict {
public:
    // Creates a dictionary with given limit. The limits is the upper bound for
    // both, the number of codewords and the length of a codeword.
    explicit MruDict (int limit);

    // Returns the codeword count and length limit.
    int limit () const;

protected:
    // This method handles the situation when the longest matching codeword is
    // found. It takes the number `i` of the longest matching codeword, and
    // does two things. Then it moves `i` and all of it's prefixes to the front // of the MRU queue and returns the number of the new codeword, which is
    // the extension of `i`.
    //
    // If the number of codewords would exceed the limit, the new codeword
    // receives the number of the least recently used codeword, which gets
    // discarded.
    //
    // If the length of the new codeword would exceed the limit, nothing is
    // actually done and the return value is `0`.
    //
    // **Note:** The indexing of codewords is 1-based.
    int match (int i);

private:
    std::list<int> m_queue;
    std::vector<int> m_parents;
    std::vector<std::list<int>::iterator> m_queue_positions;

    int m_limit;
};

inline int MruDict::limit () const {
    return m_limit;
}

#endif // MRU_POOL_HPP
