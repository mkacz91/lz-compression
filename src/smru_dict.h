#ifndef SMRU_DICT_H
#define SMRU_DICT_H

#include "prefix.h"
#include <list>
#include <vector>

#include "dict_base.h"
#include "label_tree_node.h"

// SmruDictBase
// =============================================================================
//
// Base class for the SMRU encode and decode dictionaries for use during LZ78/
// LZW factorization. Abbreviation SMRU stands for _Stongly Most Recently Used_
// and refers to the codeword discard strategy.
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
class SmruDictBase : public DictBase {
public:
    // Creates a dictionary with given limit. The limits is the upper bound for
    // both, the number of codewords and the length of a single codeword.
    explicit SmruDictBase (int limit);

    // Implements `DictBase::make_permanent(int)`.
    virtual void make_permanent (int i);

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
    // The discard queue. The elements are ordered with respect to time of use, // the least recently used codewords being in the front.
    std::list<int> m_queue;

    // The indices of codeword parents.
    std::vector<int> m_parents;

    // The positions of codewords within the discard queue.
    std::vector<std::list<int>::iterator> m_queue_positions;
};

// SmruEncodeDict
// =============================================================================
//
// The SMRU dictionary specialized for **encoding**.
class SmruEncodeDict : public SmruDictBase, public EncodeDictBase {
public:
    // Constructs a dictionary with given limit.
    SmruEncodeDict (int limit);

    // Implements `EncodeDictBase::try_char(char)`. If the resulting new
    // codeword would exceed the maximal length, it is rejected.
    virtual Match try_char (char a);

    // Implements `EncodeDictBase::fail_char(char)`.
    virtual Match fail_char ();

private:
    // The trie used to efficiently retrieve codeword matchings consists of
    // nodes labelled with codeword numbers and edges labelled with single
    // characters.
    typedef LabelTreeNode<int, char> Node;

    // The node pool. We allocate all nodes at once. The first element is the
    // the root.
    std::vector<Node> m_nodes;

    // Pointer to the current state of the automaton, i.e., the longest match.
    Node* m_node;

    // The length of the current longest match.
    int m_match_length;
};

// SmruDecodeDict
// =============================================================================
//
// The SMRU dictionary specialized for **decoding**.
class SmruDecodeDict : public SmruDictBase, public DecodeDictBase {
public:
    // Constructs a new dictionary with given limit.
    SmruDecodeDict (int limit);

    // Implements `DecodeDictBase::add_extension(int, int)`. If the resulting
    // codeword would exceed the maximal length, it is silently ignored.
    virtual void add_extension (int i, int begin);

    // Implements `DecodeDictBase::codeword(int)`.
    virtual Codeword const& codeword (int i) const;

private:
    std::vector<Codeword> m_codewords;
};

inline void SmruDecodeDict::add_extension (int i, int begin) {
    assert(0 <= i && i < m_codewords.size());

    int j = this->match(i);
    if (j != 0)
        m_codewords[j] = Codeword(begin, m_codewords[i].length + 1);
}

inline Codeword const& SmruDecodeDict::codeword (int i) const {
    return m_codewords[i];
}

// SmruDict
// =============================================================================
class SmruDict {
public:
    typedef SmruEncodeDict EncodeDict;
    typedef SmruDecodeDict DecodeDict;
};

#endif // SMRU_DICT_H
