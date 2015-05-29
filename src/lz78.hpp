#ifndef LZ78_HPP
#define LZ78_HPP

#include "prefix.hpp"

#include "buffer.hpp"

template <typename Dictionary>
class Lz78 {
public:
    // Constructs a new LZ78 encoder/decoder with a dictionary of given limit.
    // Non-positive `dictionary_limit` results in an unlimited dictionary.
    Lz78 (int dictionary_limit = 0);

    Buffer encode(Buffer const& input);

    Buffer decode(Buffer const& input);

private:
    int const m_dictionary_limit;
};

template <typename Dictionary>
inline Lz78<Dictionary>::Lz78 (int dictionary_limit) :
    m_dictionary_limit(dictionary_limit)
{
    // Do nothing.
}

template <typename Dictionary>
Buffer Lz78<Dictionary>::encode (Buffer const& input) {
    Dictionary dict;
    BufferCharReader reader(input);
    Buffer output;
    BufferBitWriter writer(output);
    int const codeword_no_length = m_dictionary_limit > 0
        ? ceil_log2(m_dictionary_limit)
        : WORD_LENGTH;

    while (!reader.eob()) {
        char a = reader.get();
        int codeword_no = dict.try_char(a);
        if (codeword_no != -1) {
            writer.put(codeword_no, codeword_no_length);
            writer.put(a, CHAR_LENGTH);
        }
    }
    // Append the 'incomplete' match.
    if (dict.peek_codeword_no() > 0)
        writer.put(dict.peek_codeword_no(), codeword_no_length);

    return output;
}

template <typename Dictionary>
Buffer Lz78<Dictionary>::decode (Buffer const& input) {
    // TODO
    return Buffer();
}

#endif // LZ78_HPP
