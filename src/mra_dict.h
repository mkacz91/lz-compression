#ifndef MRA_DICT_H
#define MRA_DICT_H

#include "prefix.h"

#include "pool_dict.h"

// MraPool
// =============================================================================
// TODO doc
class MraPool : public CodewordPool {
public:
    MraPool (int limit);

    // Implements `CodewordPool::get(Match const&)`.
    virtual int get (Match const& match);

private:
    int m_next;
};

inline MraPool::MraPool (int limit) :
    CodewordPool(limit),
    m_next(1)
{
    /* Do nothing. */
}

inline int MraPool::get (Match const& match) {
    UNUSED(match);
    int result = m_next;
    if (++m_next > limit())
        m_next -= limit();
    return result;
}

typedef PoolDictPair<MraPool> MraDictPair;

#endif // MRA_DICT_H
