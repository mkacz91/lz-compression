#ifndef TRIE_DICTIONARY_HPP
#define TRIE_DICTIONARY_HPP

#include "prefix.hpp"
#include <vector>

#include "dictionary_node.hpp"
#include "mru_pool.hpp"

class TrieDictionary {
public:
    TrieDictionary (int limit = 0);

    int try_char (char a);

private:
    typedef DictionaryNode Node;
    typedef std::vector<Node> NodeVector;

    MruPool m_codeword_pool;
    NodeVector m_nodes;
    Node* m_node;

    Node* new_node ();
};

#endif // TRIE_DICTIONARY_HPP
