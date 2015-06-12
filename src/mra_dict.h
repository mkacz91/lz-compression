#ifndef MRA_DICT_H
#define MRA_DICT_H

#include "prefix.h"

#include "pool_dict.h"

// MraPool
// =============================================================================
// TODO doc
class MraPool : public CodewordPool {
public:
    MraPool (int limit, bool single_char_codewords);

    // Implements `CodewordPool::match(int)`.
    virtual int match (int i);

private:
    int m_fixed;
    int m_next;
};

inline MraPool::MraPool (int limit, bool single_char_codewords) :
    CodewordPool(limit, single_char_codewords),
    m_fixed(single_char_codewords ? CHAR_CNT : 0),
    m_next(m_fixed + 1)
{
    /* Do nothing. */
}

inline int MraPool::match (int i) {
    UNUSED(i);
    if (m_next == limit()) {
        m_next = m_fixed + 1;
        return limit();
    } else {
        return m_next++;
    }
}

typedef PoolDictPair<MraPool> Mra;

#endif // MRA_DICT_H
