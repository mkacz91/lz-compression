#ifndef LZW_H
#define LZW_H

#include "prefix.h"

#include "lz.h"

// Lzw
// =============================================================================
//
// TODO: documentation
template <typename Dict>
class Lzw : public Lz {
public:
    // Constructs an LZW encoder/decoder with given dictionary limit. The
    // actual limit of the dictionary used is expanded by `CHAR_CNT` as this
    // particular method adds all single-letter codewords to the dictionary.
    Lzw (int dictionary_limit);

    // Implements `Lz::encode(Buffer const&) const`.
    virtual Buffer encode(Buffer const& input) const;

    // Implements `Lz::decode(Buffer const&) const`.
    virtual Buffer decode(Buffer const& output) const;

    // Implements `Lz::codeword_bits () const`.
    virtual int codeword_bits () const;
};

template <typename Dict>
Lzw<Dict>::Lzw (int dictionary_limit) :
    Lz(dictionary_limit + CHAR_CNT)
{
    /* Do nothing */
}

template <typename Dict>
Buffer Lzw<Dict>::encode (Buffer const& input) const {
    // In this method, a dictionary preoccupied with single letter codewords is
    // used.
    typename Dict::EncodeDict dict(input, m_dictionary_limit, true);
    Buffer output;
    BufferBitWriter writer(output);

    // Number of positions ahead of the encoded part.
    int ahead = 0;
    // The outer loop iterates only at the end of the processed input and is
    // due to the presence of artificial terminating character.
    while (!dict.eob()) {
        while (!dict.eob()) {
            Match match = dict.try_char();
            ++ahead;
            if (match.is_maximal()) {
                // Only the matching codeword number is written.
                writer.put(match.codeword_no, m_codeword_no_length);
                // We put back all chars up to the first unmatched one
                // inclusive.
                dict.put_back(ahead - match.length);
                ahead = 0;
            }
        }
        if (ahead != 0) {
            // We reached the end of input but still have some partially matched
            // prefix. Behave as if a special terminating char was present.
            Match match = dict.fail_char();
            writer.put(match.codeword_no, m_codeword_no_length);
            if (match.length != ahead) {
                // The match terminates before the end of input. We have to
                // prepare for another round.
                dict.put_back(ahead - match.length);
                ahead = 0;
            }
        }
    }

    return output;
}

template <typename Dict>
Buffer Lzw<Dict>::decode (Buffer const& output) const {
    // In this method, a dictionary preoccupied with single letter codewords is
    // used.
    typename Dict::DecodeDict dict(m_dictionary_limit, true);
    Buffer input;
    BufferCharWriter writer(input);
    BufferBitReader reader(output);

    // Starting position of the part not decoded yet.
    int pos = 0;
    while (!reader.eob()) {
        int i = reader.get(m_codeword_no_length);
        Codeword cw = dict.codeword(i);
        dict.add_extension(i, pos);
        if (cw.length == 1) {
            // This is a single-letter codeword. It's not a subsequence of the
            // decoded text, but its number determines the letter instead.
            writer.put(i - 1);
        } else {
            // This is an ordinary codeword. The first character of `cw` is the
            // last character of the codeword created during the preceding
            // iteration, and it wasn't known at that time. We have to
            // explicitly handle that character because in case those two
            // codewords are in fact the same, we might end up copying
            // undefined data.
            writer.put(BufferCharSlice(input, cw.begin, 1));
            writer.put(BufferCharSlice(input, cw.begin + 1, cw.length - 1));
        }
        // Notice that `pos` is advanced only by `cw.length`.
        pos += cw.length;
    }

    return input;
}

template <typename Dict>
int Lzw<Dict>::codeword_bits () const {
    return m_codeword_no_length;
}

#endif // LZW_H
