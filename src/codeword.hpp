#ifndef CODEWORD_HPP
#define CODEWORD_HPP

struct Codeword {
    int begin;
    int length;

    explicit Codeword (int begin, int length);

    inline bool operator == (Codeword const& cw) const;
};

inline Codeword::Codeword (int begin, int length) :
    begin(begin),
    length(length)
{
    // Do nothing.
}

inline bool Codeword::operator == (Codeword const& cw) const {
    return begin == cw.begin && length == cw.length;
}

#endif // CODEWORD_HPP
