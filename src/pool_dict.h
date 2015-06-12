#ifndef POOL_DICT_H
#define POOL_DICT_H

#include "prefix.h"
#include <vector>

#include "dict.h"
#include "pool_dict_tree.h"

// PoolDict
// =============================================================================
// TODO doc
template <typename Pool>
class PoolDict : public Dict {
public:
    explicit PoolDict (int limit, bool single_char_codewords);

protected:
    int match (int i);

private:
    Pool m_pool;
    int m_max_codeword_no;
};

template <typename Pool>
PoolDict<Pool>::PoolDict (int limit, bool single_char_codewords) :
    Dict(limit, single_char_codewords),
    m_pool(limit, single_char_codewords),
    m_max_codeword_no(single_char_codewords ? CHAR_CNT : 0)
{
    /* Do nothing. */
}

template <typename Pool>
inline int PoolDict<Pool>::match (int i) {
    assert(0 <= i && i <= m_max_codeword_no);
    int result = m_pool.match(i);
    m_max_codeword_no = max(result, m_max_codeword_no);
    return result;
}

// PoolEncodeDict
// =============================================================================
// TODO doc
template <typename Pool>
class PoolEncodeDict : public PoolDict<Pool>, public EncodeDict {
public:
    PoolEncodeDict (Buffer const& input, int limit, bool single_char_codewords);

    // Implements `EncodeDictBase::try_char()`.
    virtual Match try_char ();

    // Implements `EncodeDictBase::fail_char()`.
    virtual Match fail_char ();

private:
    typedef PoolDictTree::Node Node;
    typedef PoolDictTree::Edge Edge;

    PoolDictTree m_tree;
    Edge m_edge;
    Match m_match;
    int m_match_begin;
    int m_edge_pos;
};

template <typename Pool>
PoolEncodeDict<Pool>::PoolEncodeDict (
    Buffer const& input,
    int limit,
    bool single_char_codewords
) :
    PoolDict<Pool>(limit, single_char_codewords),
    EncodeDict(input),
    m_tree(input),
    m_edge(m_tree.edge_to_root()),
    m_edge_pos(0)
{
    if (single_char_codewords) {
        // Class `PoolDictTree` behaves as if the attached input were prepended
        // with the whole alphabet with negative indices.
        for (int a = 0; a < CHAR_CNT; ++a)
            m_tree.extend(0, -a - 1, a + 1);
    }
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
        // If our starting point is root, we have to initiate new match
        if (node->is_root()) {
            m_match.codeword_no = 0;
            m_match.length = 0;
            m_match_begin = this->pos();
        }
        m_edge = m_tree.edge(node, a);
        matches = !m_edge.dst->is_root();
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
            // Start another branch in the next call to `try_char()`.
            m_edge_pos = 0;
            // This may also be a full codeword match.
            if (m_edge.dst->tag.active) {
                m_match.codeword_no = m_edge.dst->tag.codeword_no;
                m_match.length += m_edge.length();
            }
        }
        return Match();
    } else {
        int new_codeword_no = this->match(m_match.codeword_no);
        if (new_codeword_no != 0)
            m_tree.extend(m_match.codeword_no, m_match_begin, new_codeword_no);
        // Start from root in the next call to `try_char()`.
        m_edge_pos = 0;
        m_edge = m_tree.edge_to_root();
        return m_match;
    }
}

template <typename Pool>
Match PoolEncodeDict<Pool>::fail_char () {
    int new_codeword_no = this->match(m_match.codeword_no);
    if (m_edge_pos != 0 && new_codeword_no != 0)
        m_tree.extend(m_match.codeword_no, m_match_begin, new_codeword_no);
    return m_match;
}

// PoolDecodeDict
// =============================================================================
// TODO doc
template <typename Pool>
class PoolDecodeDict : public PoolDict<Pool>, public DecodeDict {
public:
    PoolDecodeDict (int limit, bool single_char_codewords);

    virtual void add_extension (int i, int begin);

    virtual Codeword codeword (int i) const;

private:
    std::vector<Codeword> m_codewords;
};

template <typename Pool>
PoolDecodeDict<Pool>::PoolDecodeDict (int limit, bool single_char_codewords) :
    PoolDict<Pool>(limit, single_char_codewords),
    m_codewords(limit + 1, Codeword(0, 0))
{
    if (single_char_codewords) {
        for (int a = 0; a < CHAR_CNT; ++a)
            m_codewords[a + 1].length = 1;
    }
}

template <typename Pool>
inline void PoolDecodeDict<Pool>::add_extension (int i, int begin) {
    assert(0 <= i && i < m_codewords.size());
    int j = this->match(i);
    if (j != 0)
        m_codewords[j] = Codeword(begin, m_codewords[i].length + 1);
}

template <typename Pool>
inline Codeword PoolDecodeDict<Pool>::codeword (int i) const {
    assert(0 <= i && i < m_codewords.size());
    return m_codewords[i];
}

// CodewordPool
// =============================================================================
// TODO doc
class CodewordPool {
public:
    // Creates a new pool with given limit. If `single_char_codewords` is true
    // the pool should take into account that there is 256 permanent codewords
    // with indices from 1 to 256, one for every possible char.
    CodewordPool (int limit, bool single_char_codewords);

    int limit () const;

    virtual int match (int i) = 0;

private:
    int m_limit;
};

inline CodewordPool::CodewordPool (int limit, bool single_char_codewords) :
    m_limit(limit)
{
    assert(limit > 0);
    // Argument `single_char_codewords` is here only to bting to mind that
    // such functionality has to be taken into account.
    UNUSED(single_char_codewords);
}

inline int CodewordPool::limit () const {
    return m_limit;
}

// PoolDictPair
// =============================================================================

template <typename Pool>
class PoolDictPair {
public:
    typedef PoolEncodeDict<Pool> EncodeDict;
    typedef PoolDecodeDict<Pool> DecodeDict;
};

#endif // POOL_DICT_H
