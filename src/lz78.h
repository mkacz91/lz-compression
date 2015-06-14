#ifndef LZ78_H
#define LZ78_H

#include "prefix.h"

#include "lz.h"

// Lz78
// =============================================================================
//
// TODO: documentation
template <typename DictPair>
class Lz78 : public Lz {
public:
    // Construct an LZ78 encoder/decoder with given dictionary limit.
    Lz78 (int dictionary_limit);

    // Implements `Lz::encode(Buffer const&) const`.
    virtual Buffer encode(Buffer const& input) const;

    // Implements `Lz::decode(Buffer const&) const`.
    virtual Buffer decode(Buffer const& output) const;
};

template <typename DictPair>
Lz78<DictPair>::Lz78 (int dictionary_limit) :
    Lz(dictionary_limit)
{
    /* Do nothing */
}

template <typename DictPair>
Buffer Lz78<DictPair>::encode (Buffer const& input) const {
    typename DictPair::EncodeDict dict(input, m_dictionary_limit, false);
    Buffer output;
    BufferBitWriter writer(output);

    // Number of positions ahead of the encoded part.
    int ahead = 0;
    // The outer loop iterates only at the end of the processed input and is
    // due to the presence of artificial terminating character.
    while (!dict.eob()) {
        // Most of the work is done in this loop.
        while (!dict.eob()) {
            Match match = dict.try_char();
            ++ahead;
            if (match.is_maximal()) {
                writer.put(match.codeword_no, m_codeword_no_length);
                writer.put(match.extending_char, CHAR_BITS);
                dict.put_back(ahead - match.length - 1);
                ahead = 0;
            }
        }
        if (ahead != 0) {
            // We reached the end of input but still have some partially matched
            // prefix. Behave as if a special terminating char was present.
            // Notice that `ahead` is not incremented.
            Match match = dict.fail_char();
            writer.put(match.codeword_no, m_codeword_no_length);
            if (match.length != ahead) {
                // The match terminates before the end of input, i.e.,
                // `match.extending_char` is some valid char. We write it and
                // prepare for another round.
                writer.put(match.extending_char, CHAR_BITS);
                dict.put_back(ahead - match.length - 1);
                ahead = 0;
            }
        }
    }

    return output;
}

template <typename DictPair>
Buffer Lz78<DictPair>::decode (Buffer const& output) const {
    typename DictPair::DecodeDict dict(m_dictionary_limit, false);
    Buffer input;
    BufferCharWriter writer(input);
    BufferBitReader reader(output);

    // Starting position of the part not decoded yet.
    int pos = 0;
    while (!reader.eob()) {
        int i = reader.get(m_codeword_no_length);
        Codeword cw = dict.codeword(i);
        dict.add_extension(i, pos);
        writer.put(BufferCharSlice(input, cw.begin, cw.length));
        pos += cw.length;
        // If this was the last codeword, no extending character follows.
        if (!reader.eob()) {
            writer.put(reader.get(CHAR_BITS));
            ++pos;
        }
    }

    return input;
}

#endif // LZ78_H
