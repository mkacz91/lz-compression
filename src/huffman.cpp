#include "huffman.h"
#include <queue>

// Huffman
// =============================================================================

Buffer Huffman::encode (Buffer const& input) {
    Buffer output;
    BufferBitWriter writer(output);

    // First pass is to count the weights of each letter. And produce the code
    // tree.
    std::vector<int> weights(CHAR_CNT, 0);
    BufferCharReader wreader(input);
    while (wreader.eob())
        ++weights[wreader.get()];
    Node const* root = make_tree(weights);

    // The weights have to be stored for decoding.
    for (int w : weights)
        writer.put(w, INT_LENGTH);

    // The encoder only recognizes full characters, whereas the buffer may be of
    // arbitrary length (int bits). Therefore we must store the padding info.
    writer.put(input.size() % CHAR_LENGTH, 3);

    // Then the code tree is traversed in a BFS manner to retrieve codes for
    // all characters. The queue holds `(node, code)` triples of the nodes
    // along with their intermediate codes, i.e., paths to those nodes. The
    // final result is a code for each char represented as `(word, length)`
    // meaning that the `length` least significant bits of `word` is the code.
    std::queue<pair<Node const*, word>> queue;
    std::queue<pair<Node const*, word>> next_queue;
    std::vector<pair<word, int>> codes(CHAR_CNT);
    queue.emplace(root, NULL_WORD);
    int length = 0;
    while (!queue.empty()) {
        while (!queue.empty()) {
            auto nc = queue.front();
            Node const* node = nc.first;
            word code = nc.second;
            queue.pop();
            if (node->is_leaf()) {
                codes[node->value()] = make_pair(code, length);
            } else {
                next_queue.emplace(node->child( true), (code << 1) | 1);
                next_queue.emplace(node->child(false), (code << 1)    );
            }
        }
        swap(queue, next_queue);
        ++length;
    }

    // Now we can smooth sail and output the codes.
    BufferCharReader reader(input);
    while (reader.eob()) {
        auto cl = codes[reader.get()];
        writer.put(cl.first, cl.second);
    }

    delete root;
    return output;
}

Buffer Huffman::decode (Buffer const& output) {
    Buffer input;
    BufferCharWriter writer(input);
    BufferBitReader reader(output);

    // First we read the character weights to build the code tree.
    std::vector<int> weights(CHAR_CNT, 0);
    for (int a = 0; a < CHAR_CNT; ++a)
        weights[a] = reader.get(INT_LENGTH);
    Node const* root = make_tree(weights);

    int padding = reader.get(3);

    // Now the tree can be used as an automaton.
    Node const* node = root;
    char last_char;
    while (!reader.eob()) {
        assert(!node->is_leaf());
        node = node->child(reader.get(1));
        if (node->is_leaf()) {
            if (!reader.eob()) {
                writer.put(node->value());
                node = root;
            } else {
                last_char = node->value();
            }
        }
    }

    if (padding != 0) {
        BufferBitWriter bwriter(input);
        bwriter.put(last_char >> padding, padding);
    } else {
        writer.put(last_char);
    }

    delete root;
    return input;
}

inline bool Huffman::NodeCompare::operator () (
    Huffman::Node const* n1,
    Huffman::Node const* n2
) const {
    return n1->weight() < n2->weight();
}

Huffman::Node const* Huffman::make_tree (std::vector<int> const& weights) {
    assert (weights.size() == CHAR_CNT);

    std::priority_queue<Node const*, std::vector<Node const*>, NodeCompare>
        queue;

    for (int a = 0; a < CHAR_CNT; ++a)
        queue.push(new Node(a, weights[a]));

    while (queue.size() != 1) {
        Node const*  one_child = queue.top(); queue.pop();
        Node const* zero_child = queue.top(); queue.pop();
        queue.push(new Node(one_child, zero_child));
    }

    return queue.top();
}

// Huffman::Node
// =============================================================================

inline Huffman::Node::Node (char value, int weight) :
    m_one_child(nullptr),
    m_value(value),
    m_weight(weight)
{
    /* Do nothing. */
}

inline Huffman::Node::Node (Node const* one_child, Node const* zero_child) :
    m_one_child(one_child),
    m_zero_child(zero_child),
    m_weight(one_child->m_weight + zero_child->m_weight)
{
    /* Do nothing. */
}

Huffman::Node::~Node () {
    if (!this->is_leaf()) {
        delete m_one_child;
        delete m_zero_child;
    }
}

inline Huffman::Node const* Huffman::Node::child (bool branch) const {
    assert(!this->is_leaf());
    return branch ? m_one_child : m_zero_child;
}

inline bool Huffman::Node::is_leaf () const {
    return m_one_child == nullptr;
}

inline char Huffman::Node::value () const {
    assert(this->is_leaf());
    return m_value;
}

inline int Huffman::Node::weight () const {
    return m_weight;
}
