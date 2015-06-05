#ifndef LZ_H
#define LZ_H

#include "prefix.h"

#include "buffer.h"
#include "dict_base.h"

// Lz
// =============================================================================
//
class Lz {
public:
    // Constructs a new LZ encoder/decoder with a dictionary of given limit.
    // Non-positive `dictionary_limit` results in an unlimited dictionary.
    Lz (int dictionary_limit);

    virtual Buffer encode (Buffer const& input) const = 0;

    virtual Buffer decode (Buffer const& output) const = 0;

protected:
    // TODO: Naming
    int const m_dictionary_limit;
    int const m_codeword_no_length;
};

inline Lz::Lz (int dictionary_limit) :
    m_dictionary_limit(max(0, dictionary_limit)),
    m_codeword_no_length(
        m_dictionary_limit > 0
            ? ceil_log2(m_dictionary_limit)
            : WORD_LENGTH
    )
{
    /* Do nothing */
}

#endif // LZ_H
