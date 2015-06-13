#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "prefix.h"
#include <vector>

#include "buffer.h"

// Huffman
// =============================================================================
//
// TODO doc
class Huffman {
public:
    static Buffer encode (Buffer const& input);

    static Buffer decode (Buffer const& output);

private:
    class Node {
    public:
        Node (char value, int weight);

        Node (Node const* one_child, Node const* zero_child);

        ~Node ();

        Node const* child (bool branch) const;

        bool is_leaf () const;

        char value () const;

        int weight () const;

    private:
        Node const* m_one_child;

        Node const* m_zero_child;

        char m_value;

        int m_weight;
    };

    struct NodeCompare {
        bool operator () (Node const* n1, Node const* n2) const;
    };

    static Node const* make_tree (std::vector<int> const& weights);
};

#endif // HUFFMAN_H
