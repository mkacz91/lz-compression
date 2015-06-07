#ifndef BUFFER_H
#define BUFFER_H

#include "prefix.h"
#include <vector>

// Buffer
// =============================================================================
//
// A buffer for data. Cannot be accessed directly; designated readers and
// writers are needed.
class Buffer {
public:
    // Constructs an empty buffer.
    Buffer ();

    Buffer (Buffer&& buffer);

    ~Buffer ();

    // Returns number of bits stored in the buffer.
    int size () const;

    // Returns `true` if the contents of `buffer` are equal to the contents of
    // this buffer.
    bool operator == (Buffer const& buffer) const;

private:
    // The underlying data array.
    word* m_data;

    // Number of words allocated by `m_data`.
    int m_capacity;

    // Number of initial words of `m_data` that are in use.
    int m_open_word_cnt;

    // Size of the buffer in bits.
    int m_size;

    // Opens a new cell of `m_data` and initiates it with value `data`.
    void push_back (word data);

    // Makes sure `m_data` is able to store given number of words. Reallocates
    // if necessary and returns the pointer to the old `m_data` which has to be
    // released by the caller. If no reallocation is done, `nullptr` is
    // returned.
    //
    // The reason not to release the old `m_data` automatically is to allow
    // additional processing between the creation of new `m_data` and the
    // deletion of the old one. This is used in
    // `BufferCharWriter::put(BufferCharSlice)`.
    word* adjust_capacity (int word_cnt);

    friend class BufferBitReader;
    friend class BufferBitWriter;
    friend class BufferCharReader;
    friend class BufferCharWriter;
    friend class BufferCharSlice;

    friend std::ostream& operator << (std::ostream& ostr, Buffer const& buffer);
};

inline int Buffer::size () const {
    return m_size;
}

// Pretty printing of buffer in binary form.
std::ostream& operator << (std::ostream& ostr, Buffer const& buffer);

// BufferBitReader
// =============================================================================
//
// A buffer reader that allows reading individual bits.
class BufferBitReader {
public:
    // Constructs a bit reader attached to given buffer.
    //
    // **Warning:** The reader is valid only for as long as `buffer` is not
    // altered.
    explicit BufferBitReader (Buffer const& buffer);

    // Rteurns the next `bit_cnt` bits of the buffer and advances the read
    // position.
    word get (int bit_cnt);

    // Returns `true` if there is no more data to read.
    bool eob () const;

private:
    // The data array of the attached buffer.
    word const* const m_data;

    // Number of words left to read.
    int m_left;

    // Index of the current word within `m_data` to start reading from on a call
    // to `get()`.
    int m_pos;

    // Index of the least significant bit within the current word (indicated by
    // `m_pos`) to start reading from on a call to `get()`.
    int m_offset;
};

inline bool BufferBitReader::eob () const {
    return m_left <= 0;
}

// BufferBitWriter
// =============================================================================
//
// A buffer writer that allows appending individual bits.
class BufferBitWriter {
public:
    // Constructs a bit writer attached to given buffer.
    BufferBitWriter (Buffer& buffer);

    // Appends `bit_count` least significant bits of `data` to the buffer. The
    // value `bit_count` may not exceed `WORD_LENGTH`.
    void put (word data, int bit_cnt);

private:
    // The attached buffer.
    Buffer& m_buffer;

    // Index of the current word within `m_buffer.m_data` to start writing to on
    // a call to `put()`.
    int m_pos;

    // Index of the least significant bit within the current word (indicated by
    // `m_pos`) to start writing to on a call to `put()`.
    int m_offset;
};

// BufferCharReader
// =============================================================================
//
// A buffer reader that allows reading char by char.
class BufferCharReader {
public:
    // Constructs a char reader attached to given buffer.
    //
    // **Warning:** The reader is valid only for as long as `buffer` is not
    // altered.
    explicit BufferCharReader (Buffer const& buffer);

    // Returns the next char from the attached buffer.
    char get ();

    // Decreases the read position by `char_cnt` characters.
    void put_back (int char_cnt);

    // Returns `true` if there is no more data to read.
    bool eob () const;

private:
    // The data array of the attached buffer.
    char const* const m_data;

    // Number of characters stored in `m_data`.
    int const m_char_cnt;

    // Index of the current char within `m_data` to be read from on a call to
    // `get()`. Cannot exceed `m_char_cnt`.
    int m_pos;
};

inline char BufferCharReader::get () {
    assert(m_pos < m_char_cnt);
    return m_data[m_pos++];
}

inline void BufferCharReader::put_back (int char_cnt) {
    assert(0 <= char_cnt && char_cnt <= m_pos);
    m_pos -= char_cnt;
}

inline bool BufferCharReader::eob () const {
    return m_pos >= m_char_cnt;
}

// BufferCharSlice
// =============================================================================
//
// A slice of buffer data interpreted with char resolution.
class BufferCharSlice {
public:
    // Constructs a new slice of given length, starting at char with index
    // `begin` and ending. Providing `length <= 0` results in an empty slice.
    //
    // **Warning:** The slice is only valid for as long as `buffer` is not
    // altered.
    BufferCharSlice (Buffer const& buffer, int begin, int length);

private:
    // Starting address of the slice. This points directly into the `m_data`
    // array of the origin buffer.
    char const* const m_begin;

    // Length of the slice (in chars).
    int const m_length;

    friend class BufferCharWriter;
};

inline BufferCharSlice::BufferCharSlice (
    Buffer const& buffer,
    int begin,
    int length
) :
    m_begin(reinterpret_cast<char const*>(buffer.m_data) + begin),
    m_length(max(0, length))
{
    assert(begin < buffer.m_size / CHAR_LENGTH || m_length == 0);
    assert(begin + m_length <= buffer.m_size / CHAR_LENGTH);
}

// BufferCharWriter
// =============================================================================
//
// A buffer writer that allows writing with char resolution.
class BufferCharWriter {
public:
    // Constructs a char writer attached to given buffer.
    explicit BufferCharWriter (Buffer& buffer);

    // Appends a char to the buffer.
    void put (char data);

    // Appends a string to the buffer.
    void put (string const& data);

    // Appends a slice to the buffer. This operation is valid even if `slice`
    // comes from the same buffer it is being written to.
    void put (BufferCharSlice const& slice);

private:
    // The attached buffer.
    Buffer& m_buffer;

    // Index of the current character within `m_buffer.m_data` to start writing
    // to on a call to `put()`.
    int m_pos;
};

#endif // BUFFER_H
