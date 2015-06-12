#ifndef SMRU_DICT_H
#define SMRU_DICT_H

#include "prefix.h"
#include <list>
#include <vector>

#include "dict.h"
#include "pool_dict.h"
#include "word_tree_node.h"

// SmruPool
// =============================================================================
//
// Codeword pool for the SMRU encode and decode dictionaries for use during
// LZ78/ LZW factorization. Abbreviation SMRU stands for _Stongly Most Recently
// Used_.
//
// A SMRU dictionary is a dictionary of limited size that maintains only
// a specified number of the most recently used codewords. The word _strongly_
// means that whenever a codeword is used, all its prefixes are also considered
// used. This ensures that only the _leaf codewords_, i.e., codewords that are
// not prefixes of any other codeword, are discarded. Since every codeword must
// be accompanied by all its prefixes, the maximal codeword count is also the
// upper bound fo the length of a single codeword.
//
// Such approach enables us to store the codewords in a trie, which grants
// efficient storage, match lookup and the overall linear time of the LZ
// encoding.
class SmruPool : public CodewordPool {
public:
    // Creates a pool with given limit. The limits is the upper bound for
    // both, the number of codewords and the length of a single codeword.
    explicit SmruPool (int limit, bool single_char_codewords);

    // Implements `CodewordPool::match(int)`.
    //
    // This method handles the situation when the longest matching codeword is
    // found. It takes the number `i` of the longest matching codeword. Then it
    // moves `i` and all of it's prefixes to the back of the discard queue and
    // returns the number of the new codeword, which is the extension of `i`.
    //
    // If the number of codewords would exceed the limit, the new codeword
    // receives the number of the least recently used codeword, which gets
    // discarded.
    //
    // If the length of the new codeword would exceed the limit, nothing is
    // actually done and the return value is `0`.
    int match (int i);

private:
    // The discard queue. The elements are ordered with respect to time of use,
    // the least recently used codewords being in the front.
    std::list<int> m_queue;

    // The indices of codeword parents.
    std::vector<int> m_parents;

    // The positions of codewords within the discard queue.
    std::vector<std::list<int>::iterator> m_queue_positions;

    // The number of codewords stored in the dictionary.
    int m_size;
};

// SmruEncodeDict
// =============================================================================
//
// The SMRU dictionary specialized for **encoding**.
class SmruEncodeDict : public PoolDict<SmruPool>, public EncodeDict {
public:
    // Constructs a dictionary with given limit.
    SmruEncodeDict (Buffer const& input, int limit, bool single_char_codewords);

    // Implements `EncodeDict::try_char(char)`. If the resulting new codeword
    // would exceed the maximal length, it is rejected.
    virtual Match try_char ();

    // Implements `EncodeDict::fail_char()`.
    virtual Match fail_char ();

private:
    // TODO doc
    typedef WordTreeNode<int> Node;

    // The node pool. We allocate all nodes at once. The first element is the
    // the root.
    std::vector<Node> m_nodes;

    // Pointer to the current state of the automaton, i.e., the longest match.
    Node* m_node;

    // The length of the current longest match.
    int m_match_length;
};

struct Smru {
    typedef SmruEncodeDict EncodeDict;
    typedef PoolDecodeDict<SmruPool> DecodeDict;
};

#endif // SMRU_DICT_H
