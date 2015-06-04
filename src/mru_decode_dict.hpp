#ifndef MRU_DECODE_DICT_HPP
#define MRU_DECODE_DICT_HPP

#include "prefix.hpp"

#include "mru_dict.hpp"
#include "codeword.hpp"

class MruDecodeDict : public MruDict {
public:
    // Constructs a new dictionary with given limit.
    MruDecodeDict (int limit);

    // Adds new codeword to the dictionary. The new codeword starts at `begin`
    // and is a one-letter extension of the existing codeword `i`. If the
    // resulting codeword is longer than the limit, this function has no effect.
    void add_extension (int i, int begin);

    // Returs the `i`th codeword.
    Codeword const& codeword(int i) const;

private:
    std::vector<Codeword> m_codewords;
};

inline MruDecodeDict::MruDecodeDict (int limit) :
    MruDict(limit),
    m_codewords(limit + 1, Codeword(0, 0))
{
    // Do nothing.
}

inline void MruDecodeDict::add_extension (int i, int begin) {
    assert(0 <= i && i < m_codewords.size());

    int j = this->match(i);
    if (j != 0)
        m_codewords[j] = Codeword(begin, m_codewords[i].length + 1);
}

inline Codeword const& MruDecodeDict::codeword (int i) const {
    return m_codewords[i];
}

#endif // MRU_DECODE_DICT_HPP
