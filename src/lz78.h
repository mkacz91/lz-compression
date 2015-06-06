#ifndef LZ78_H
#define LZ78_H

#include "prefix.h"

#include "lz_base.h"

// Lz78
// =============================================================================
//
// TODO: documentation
template <typename Dict>
class Lz78 : public LzBase {
public:
    // Construct an LZ78 encoder/decoder with given dictionary limit.
    Lz78 (int dictionary_limit);

    // Implements `LzBase::encode(Buffer const&) const`.
    virtual Buffer encode(Buffer const& input) const;

    // Implements `LzBase::decode(Buffer const&) const`.
    virtual Buffer decode(Buffer const& output) const;
};

template <typename Dict>
Lz78<Dict>::Lz78 (int dictionary_limit) :
    LzBase(dictionary_limit)
{
    /* Do nothing */
}

template <typename Dict>
Buffer Lz78<Dict>::encode (Buffer const& input) const {
    typename Dict::EncodeDict dict(m_dictionary_limit);
    Buffer output;
    BufferCharReader reader(input);
    BufferBitWriter writer(output);

    // Number of positions ahead of the encoded part.
    int ahead = 0;
    // The outer loop iterates only at the end of the processed input and is
    // due to the presence of artificial terminating character.
    while (!reader.eob()) {
        // Most of the work is done in this loop.
        while (!reader.eob()) {
            char a = reader.get();
            ++ahead;
            Match match = dict.try_char(a);
            if (match.is_maximal()) {
                writer.put(match.codeword_no, m_codeword_no_length);
                writer.put(match.extending_char, CHAR_LENGTH);
                reader.put_back(ahead - match.length - 1);
                ahead = 0;
            }
        }
        // Behave as if a special terminating char was present.
        Match match = dict.fail_char();
        writer.put(match.codeword_no, m_codeword_no_length);
        if (match.length != ahead) {
            writer.put(match.extending_char, CHAR_LENGTH);
            reader.put_back(ahead - match.length);
            ahead = 0;
        }
    }

    return output;
}

template <typename Dict>
Buffer Lz78<Dict>::decode (Buffer const& output) const {
    typename Dict::DecodeDict dict(m_dictionary_limit);
    Buffer input;
    BufferCharWriter writer(input);
    BufferBitReader reader(output);

    // Starting position of the part not yet decoded.
    int pos = 0;
    while (!reader.eob()) {
        int i = reader.get(m_codeword_no_length);
        Codeword cw = dict.codeword(i);
        dict.add_extension(i, pos);
        writer.put(BufferCharSlice(input, cw.begin, cw.length));
        pos += cw.length;
        // If this was the last codeword, no extending character follows.
        if (!reader.eob()) {
            writer.put(reader.get(CHAR_LENGTH));
            ++pos;
        }
    }

    return input;
}

#endif // LZ78_H
