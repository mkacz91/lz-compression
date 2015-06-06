#ifndef DICT_BASE_H
#define DICT_BASE_H

#include "prefix.h"

// **Note:** The classes defined here do not serve the purpose of making it
// possible to use dictionaries in a polymorphic way (they are intended to be
// embedded in the LZ78/LZW factorization algorithms by means templatization).
// They exist rather to impose interface and provide a place for common
// documentation.

// DictBase
// =============================================================================
//
// Base class for all ditionaries to be used by LZ78/LZW encoders/decoders.
class DictBase {
public:
    // Creates an empty dictionary with given limit, which is the upper bound
    // for the number of codewords. Unlimited dictionaries should set `limit`
    // to `0`.
    explicit DictBase (int limit);

    // Returns the number of codewords stored in the dictionary. Always
    // `<= limit()`.
    int size () const;
    
    // Returns the upper bound for the number of codewords. Always 0 for
    // unlimited dictionaries.
    int limit () const;

    // Marks the codeword with number `i` as permanent. Limited dictionaries
    // are forbidden to remove such codewords.
    virtual void make_permanent (int i) = 0;

protected:
    // The upper bound for the number of codewords. Irrelevant in unlimited
    // dictionaries and equal to 0.
    int const m_limit;
    
    // Returns a fresh codeword number, effectively increasing the size of the
    // dictionary by one. If the limit has been reached before calling this
    // method, the result is 0 and the size is not increased.
    int fresh_codeword_no ();
    
private:
    // Number of codewords stored in the dictionary. Derived classes should
    // update it using `fresh_codeword_no()`.
    int m_size;
};

inline DictBase::DictBase (int limit) :
    m_limit(limit),
    m_size(0)
{
    assert(limit >= 0);
}

inline int DictBase::size() const {
    return m_size;
}

inline int DictBase::limit () const {
    return m_limit;
}

inline int DictBase::fresh_codeword_no () {
    return m_size < m_limit ? ++m_size : 0;
}

// Match
// =============================================================================
//
// Info about maximal match during the encoding phase. Carries the index and
// length of the longest matching codeword as well as the first character that
// didn't match. That last character is called `extending_char` as it is the
// added to the longest match to form a new codeword.
//
// Instances of `Match` have to be returned even if no maximal match was
// found and therefore no new codeword is created. For that purpose, a special,
// otherwise invalid, value is permitted, namely `codeword_no = -1`. The other
// fields are unspecified in such case.
struct Match {
    int codeword_no;
    int length;
    char extending_char;

    // Constructs a new match with given codeword number and length.
    Match (int codeword_no, int length, char extending_char);

    // Constructs a non-maximal match.
    Match ();

    // Returns `true` if this is a maximal match.
    bool is_maximal () const;

    // Retuns `true` if `match` is equal to this match.
    bool operator == (Match const& match) const;
};

inline Match::Match (int codeword_no, int length, char extending_char) :
    codeword_no(codeword_no),
    length(length),
    extending_char(extending_char)
{
    assert(codeword_no >= 0 && length >= 0);
}

inline Match::Match () :
    // Other values undefined.
    codeword_no(-1)
{
    /* Do nothing */
}

inline bool Match::is_maximal () const {
    return codeword_no != -1;
}

inline bool Match::operator == (Match const& match) const {
    return codeword_no == match.codeword_no
        && length == match.length
        && extending_char == match.extending_char;
}

// EncodeDictBase
// =============================================================================
//
// Base class for all dictionaries specialized for **encoding**. Derived class
// should also inherit from `DictBase`.
//
// The dictionary behaves like a growing automaton. It exposes a method named
// `try_char()` which emits codeword number whenever the maximum match is found.
class EncodeDictBase {
public:
    // Advances the internal state of the automaton, trying to match the
    // currently matched codeword extended by another letter, i.e., `a`.
    //
    // If there is a match, the result is a _non maximal_ instance of `Match`,
    // which can be checked with `Match::is_maximal()`. If the provided char
    // cannot be used to extend the current match, a maximal match is yielded.
    //
    // On success, a new codeword is added. In limited dictionaries this may
    // involve discarding some other codeword or rejecting the new one. Unless
    // the limit has been reached the codewords should receive the consecutive
    // integers as indices, starting from 1. Codeword number 0 is the special
    // _empty codeword_.
    virtual Match try_char (char a) = 0;

    // This methods acts as if `try_char()` was called with a special character
    // that hasn't and won't occur in the processed text. This is to allow
    // putting an unique character indicating _end of input_.
    //
    // **Warning:** If the `extending_char` field of the resulting `Match` is
    // the imaginary unique character, it is obviously impossible to express it
    // in terms of the `char` datatype. The caller must rely on the context
    // of computation to determina if it's safe to use that value.
    //
    // __I was considering setting the type of the argument to `try_char()` to
    // `int` which would allow passing the artificial character and
    // `fail_char()` woudln't be needed. But this would introduce some
    // additional branching in `try_char()`, which could affect performance as
    // that method is called very often. If someone feels like implementing and
    // testing that approach, he or she is very welcome to do so.__
    virtual Match fail_char () = 0;
};

// Codeword
// =============================================================================
//
// Representation of a codeword. A codeword is a subsequence of the input.
// Such subsequence is determined by its starting index `begin` and its
// `length`.
struct Codeword {
    int begin;
    int length;

    // Creates a new codeword with given starting index and length.
    Codeword (int begin, int length);

    // Returns `true` if `cw` is equal to this codeword.
    inline bool operator == (Codeword const& cw) const;
};

inline Codeword::Codeword (int begin, int length) :
    begin(begin),
    length(length)
{
    /* Do nothing */
}

inline bool Codeword::operator == (Codeword const& cw) const {
    return begin == cw.begin && length == cw.length;
}

// DecodeDictBase
// =============================================================================
//
// Base class for all dictionaries specialized for **decoding**. Derived class
// should also inherit from `DictBase`.
class DecodeDictBase {
    // Adds new codeword to the dictionary. The new codeword starts at `begin`
    // and is a one-letter extension of the existing codeword `i`. Unless the
    // limit has been reached the codewords should receive the consecutive
    // integers as indices, starting from 1.
    virtual void add_extension (int i, int begin) = 0;

    // Returns the `i`th codeword.
    virtual Codeword const& codeword (int i) const = 0;
};

#endif // DICT_BASE_H
