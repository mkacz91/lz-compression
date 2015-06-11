#ifndef DICT_H
#define DICT_H

#include "prefix.h"

#include "buffer.h"

// **Note:** The classes defined here do not serve the purpose of making it
// possible to use dictionaries in a polymorphic way (they are intended to be
// embedded in the LZ78/LZW factorization algorithms by means templatization).
// They exist rather to impose interface and provide a place for common
// documentation.

// Dict
// =============================================================================
//
// Base class for all ditionaries to be used by LZ78/LZW encoders/decoders.
class Dict {
public:
    // Creates an empty dictionary with given limit, which is the upper bound
    // for the number of codewords. Unlimited dictionaries should set `limit`
    // to `0`.
    // TODO
    Dict (int limit, bool single_char_codewords);

    // Returns the upper bound for the number of codewords. Always 0 for
    // unlimited dictionaries.
    int limit () const;

protected:
    // The upper bound for the number of codewords. Irrelevant in unlimited
    // dictionaries and equal to 0.
    int const m_limit;
};

inline Dict::Dict (int limit, bool single_char_codewords) :
    m_limit(limit)
{
    // The argument `single_char_codewords` is present only to bring to mind
    // that such ffeature must be implemented in base class.
    UNUSED(single_char_codewords);
    assert(limit >= 0);
}

inline int Dict::limit () const {
    return m_limit;
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
// TODO
struct Match {
    int codeword_no;
    int length;
    char extending_char;

    // Constructs a non-maximal match
    Match ();

    // Constructs a maximal match.
    Match (int codeword_no, int length, char extending_char);

    bool is_maximal () const;

    bool operator == (Match const& match) const;
};

inline Match::Match () :
    // Other values undefined.
    codeword_no(-1)
{
    /* Do nothing */
}

inline Match::Match (int codeword_no, int length, char extending_char) :
    codeword_no(codeword_no),
    length(length),
    extending_char(extending_char)
{
    assert(codeword_no >= 0 && length >= 0);
}

inline bool Match::is_maximal () const {
    return codeword_no != -1;
}

inline bool Match::operator == (Match const& match) const {
    return (
            !this->is_maximal() &&
            !match.is_maximal()
        ) || (
            codeword_no == match.codeword_no &&
            length == match.length &&
            extending_char == match.extending_char
        );
}

// EncodeDict
// =============================================================================
//
// Base class for all dictionaries specialized for **encoding**. Derived class
// should also inherit from `Dict`.
//
// The dictionary behaves like a growing automaton. It exposes a method named
// `try_char()` which emits codeword number whenever the maximum match is found.
class EncodeDict {
public:
    // TODO doc
    EncodeDict (Buffer const& input);

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
    // TODO
    virtual Match try_char () = 0;

    // This methods acts as if `try_char()` was called with a special character
    // that hasn't and won't occur in the processed text. This allows simulting // an unique character indicating _end of input_, but more importantly,
    // allows different behaviour depending on encoding method.
    virtual Match fail_char () = 0;

    // TODO doc
    void put_back (int char_cnt);

    bool eob () const;

protected:
    // TODO doc
    char get_char ();

private:
    // TODO doc
    BufferCharReader m_reader;
};

inline EncodeDict::EncodeDict (Buffer const& input) :
    m_reader(input)
{
    /* Do nothing. */
}

inline void EncodeDict::put_back (int char_cnt) {
    m_reader.put_back(char_cnt);
}

inline bool EncodeDict::eob () const {
    return m_reader.eob();
}

inline char EncodeDict::get_char () {
    return m_reader.get();
}

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

// DecodeDict
// =============================================================================
//
// Base class for all dictionaries specialized for **decoding**. Derived class
// should also inherit from `Dict`.
class DecodeDict {
    // Adds new codeword to the dictionary. The new codeword starts at `begin`
    // and is a one-letter extension of the existing codeword `i`. Unless the
    // limit has been reached the codewords should receive the consecutive
    // integers as indices, starting from 1.
    virtual void add_extension (int i, int begin) = 0;

    // Returns the `i`th codeword.
    virtual Codeword codeword (int i) const = 0;
};

// DictPair
// =============================================================================
//
// TODO
template <typename ED, typename DD>
class DictPair {
public:
    typedef ED EncodeDict;
    typedef DD DecodeDict;
};

#endif // DICT_H
