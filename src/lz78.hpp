#ifndef LZ78_HPP
#define LZ78_HPP

#include "prefix.hpp"

#include "lz.hpp"

// TODO: documentation
template <typename Dict>
class Lz78 : public Lz {
public:
    using Lz::Lz;

    virtual Buffer encode(Buffer const& input) const override;

    virtual Buffer decode(Buffer const& output) const override;
};

template <typename Dict>
Buffer Lz78<Dict>::encode (Buffer const& input) const {
    typename Dict::EncodeDict dict(m_dictionary_limit);
    Buffer output;
    BufferCharReader reader(input);
    BufferBitWriter writer(output);

    while (!reader.eob()) {
        char a = reader.get();
        int codeword_no = dict.try_char(a);
        if (codeword_no != -1) {
            writer.put(codeword_no, m_codeword_no_length);
            writer.put(a, CHAR_LENGTH);
        }
    }
    // Append the 'incomplete' match.
    if (dict.peek_codeword_no() > 0)
        writer.put(dict.peek_codeword_no(), m_codeword_no_length);

    return output;
}

template <typename Dict>
Buffer Lz78<Dict>::decode (Buffer const& output) const {
    typename Dict::DecodeDict dict(m_dictionary_limit);
    Buffer input;
    BufferCharWriter writer(input);
    BufferBitReader reader(output);

    int size = 0;
    while (!reader.eob()) {
        int codeword_no = reader.get(m_codeword_no_length);
        Codeword cw = dict.codeword(codeword_no);
        dict.add_extension(codeword_no, size);
        writer.put(BufferCharSlice(input, cw.begin, cw.length));
        size += cw.length;
        if (!reader.eob()) {
            writer.put(reader.get(CHAR_LENGTH));
            ++size;
        }
    }

    return input;
}

#endif // LZ78_HPP
