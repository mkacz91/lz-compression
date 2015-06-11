#ifndef POOL_DICT_H
#define POOL_DICT_H

#include "prefix.h"
#include <vector>

#include "dict.h"

// PoolDict
// =============================================================================
// TODO doc
template <typename Pool>
class PoolDict : public Dict {
public:
    explicit PoolDict (int limit, bool single_char_codewords);

private:
    Pool m_pool;
};

template <typename Pool>
PoolDict<Pool>::PoolDict (int limit, bool single_char_codewords) :
    DictBase(limit, single_char_codewords),
    m_pool(limit)
{
    /* Do nothing. */
}

// PoolEncodeDict
// =============================================================================
// TODO doc
template <typename Pool>
class PoolEncodeDict : public PoolDict<Pool>, public EncodeDict {
public:
    PoolEncodeDict (int limit, bool single_char_codewords);

    // Implements `EncodeDictBase::try_char()`.
    virtual Match try_char ();

    // Implements `EncodeDictBase::fail_char()`.
    virtual Match fail_chat ();

private:
    PoolDictTree m_tree;
    PoolDictTree::Edge m_edge;
    Match m_match;
    int m_match_begin;
    int m_edge_pos;
};

template <typename Pool>
PoolEncodeDict<Pool>::PoolEncodeDict (int limit, bool single_char_codewords) {
    // TODO
}

template <typename Pool>
Match PoolEncodeDict<Pool>::try_char () {
    char a = this->get_char();
    bool matches = false;
    if (m_edge_pos == 0) {
        // No intermediate match is in progress. A full match or a failure was
        // found in the preceding call to `try_char()`. We have to choose a new
        // branch to go to.
        Node const* node = m_edge.dst;
        // If our starting point is root, we have to remember the beginning of
        // any match that follows.
        if (node->is_root())
            m_match_begin = this->pos();
        m_edge = m_tree.edge(node, a);
        matches = (m_edge.length() != 0);
        // If this is not just a bifurcation, we just got the missing char for
        // our match.
        if (m_edge.dst->tag.active)
            m_match.extending_char = a;
    } else {
        matches = (m_edge[m_edge_pos] == a);
    }

    if (matches) {
        ++m_edge_pos;
        if (m_edge_pos == m_edge.length()) {
            // This causes going another branch in the next call `try_char()`.
            m_edge_pos = 0;
            // This may also be a full codeword match.
            if (m_edge.dst->tag.active) {
                m_match.codeword_no = m_edge.dst->tag.codeword_no;
                m_match.length += m_edge.length();
            }
        }
        return Match();
    } else {
        int new_codeword_no = m_pool.get(m_match);
        m_tree.extend(m_match.codeword_no, m_match_begin, new_codeword_no);
        return m_match;
    }
}

template <typename Pool>
Match PoolEncodeDict<Pool>::fail_char () {
    // TODO
}

#endif // POOL_DICT_H
