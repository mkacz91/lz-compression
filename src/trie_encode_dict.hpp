#ifndef TRIE_ENCODE_DICT_HPP
#define TRIE_ENCODE_DICT_HPP

#include "prefix.hpp"
#include <vector>

#include "encode_dict_node.hpp"
#include "mru_pool.hpp"

// A dictionary for usage in LZ78/LZW factorization. It holds codewords in
// a trie-like datastructure. Can be limited or unlimited.
//
// In the limited case, the least recently used entries are discarded when
// needed. This strategy ensures that only leaves are removed, maintaining the
// trie property.
//
// The dictionary behaves like a growing automaton. The only method it exposes
// is `try_char()` which emits codeword number whenever the maximum match is
// found.
class TrieEncodeDict {
public:
    typedef MruPool CodewordPool;

    // Constructs a dictionary with given limit for the number of codewords.
    // Non-positive `limit` results in an unlimited dictionary.
    TrieEncodeDict (int limit = 0);

    ~TrieEncodeDict ();

    // Advances the internal state of the automaton, trying to match the
    // the currently matched word extended by another letter `a`.
    //
    // If there is a match, the result is -1, meaning that a longer match may
    // exist. Non-negative result indicates success. It means that the word
    // composed of characters provided after the last successful call to
    // `try_char()`, excluding the current one, is the longest matching
    // codeword present in the dictionary. The result is then the number of the
    // matching codeword, or 0 for the empty codeword.
    //
    // On success, a new codeword is added; possibly in place of the least
    // recently used one.
    int try_char (char a);

    // Returns the number of the currently matched codeword, that may still be
    // extended.
    int peek_codeword_no () const;

private:
    typedef EncodeDictNode Node;
    typedef std::vector<Node> NodeVector;

    // A pool of codeword numbers, maintaining the MRU queue. Note that it is
    // 0-based, unlike the codeword numbers associated stored in nodes. The
    // value 0 is reserved for the root and is not subject to queueing.
    CodewordPool m_codeword_pool;

    // The node pool. If this dictionary is limited, we allocate all nodes at
    // once to assure memory coherence. In an unlimited dictionary, this field
    // ends up practically unused. The first element is the root in both cases.
    NodeVector m_nodes;

    // Pointer to the current state of the automaton, i.e., the longest match.
    Node* m_node;
};

inline int TrieEncodeDict::peek_codeword_no () const {
    return m_node->codeword_no();
}

#endif // TRIE_ENCODE_DICT_HPP
