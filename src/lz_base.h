#ifndef LZ_BASE_H
#define LZ_BASE_H

#include "prefix.h"

#include "buffer.h"
#include "dict_base.h"

// LzBase
// =============================================================================
//
// Base class for LZ encoders/decoders.
class LzBase {
public:
    // Constructs a new LZ encoder/decoder with a dictionary of given limit.
    LzBase (int dictionary_limit);

    // Encode the `input` buffer interpreted as sequence of chars.
    virtual Buffer encode (Buffer const& input) const = 0;

    // Decode the `output` buffer. The resulting buffer is can be read as
    // a sequence of chars. This is an iverse operation to `encode()`.
    virtual Buffer decode (Buffer const& output) const = 0;

protected:
    // TODO: Naming
    int const m_dictionary_limit;
    int const m_codeword_no_length;
};

inline LzBase::LzBase (int dictionary_limit) :
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