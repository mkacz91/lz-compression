#ifndef MRU_ENCODE_DICT_HPP
#define MRU_ENCODE_DICT_HPP

#include "prefix.hpp"
#include <vector>

#include "encode_dict_node.hpp"
#include "mru_dict.hpp"

// A limited capacity dictionary for usage in LZ78/LZW **encoding**. The
// dictionary maintains a limited number of the most recently used (abbr. MRU)
// codewords.
//
// The dictionary behaves like a growing automaton. It exposes a method named
// `try_char()` which emits codeword number whenever the maximum match is found.
class MruEncodeDict : public MruDict {
public:
    // Constructs a dictionary with given limit. The limit is an upper bound for
    // both, the number of codewords and the length of a single codeword.
    MruEncodeDict (int limit);

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
    // recently used one. If the length of the codeword to be added excedes
    // the upper bound, the dictionary isn't altered.
    int try_char (char a);

    // Returns the number of the currently matched codeword, that may still be
    // extended.
    int peek_codeword_no () const;

private:
    typedef EncodeDictNode Node;

    // The node pool. We allocate all nodes at once. The first element is the
    // the root
    std::vector<Node> m_nodes;

    // Pointer to the current state of the automaton, i.e., the longest match.
    Node* m_node;
};

inline int MruEncodeDict::peek_codeword_no () const {
    return m_node->codeword_no();
}

#endif // MRU_ENCODE_DICT_HPP
