#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "prefix.hpp"
#include <vector>

// A buffer for data. Cannot be accessed directly; designated readers and
// writers are needed.
class Buffer {
public:
    // Constructs an empty buffer.
    Buffer ();

    Buffer (Buffer&& buffer);

    // Returns number of bits stored in the buffer.
    int size () const;

    bool operator == (Buffer const& buffer) const;

private:
    std::vector<word> m_buffer;
    int m_size;

    friend class BufferBitReader;
    friend class BufferBitWriter;
    friend class BufferCharReader;
    friend class BufferCharWriter;
    friend class BufferCharSlice;

    friend std::ostream& operator << (std::ostream& ostr, Buffer const& buffer);
};

std::ostream& operator << (std::ostream& ostr, Buffer const& buffer);

// A buffer reader that allows reading individual bits.
class BufferBitReader {
public:
    // Constructs a bit reader attached to given buffer.
    BufferBitReader (Buffer const& buffer);

    // Rteurns the next `bit_count` bits of the buffer and advances the read
    // position.
    word get (int bit_cnt);

    // Returns `true` if there is no more data to read.
    bool eob () const;

private:
    word const* const m_buffer;
    int m_left;
    int m_pos;
    int m_offset;
};

// A buffer writer that allows appending individual bits.
class BufferBitWriter {
public:
    // Constructs a bit writer attached to given buffer.
    BufferBitWriter (Buffer& buffer);

    // Appends `bit_count` rightmost bits pf `data` to the buffer. The value
    // `bit_count` may not exceed `WORD_LENGTH`.
    void put (word data, int bit_cnt);

private:
    std::vector<word>& m_buffer;
    int& m_size;
    int m_pos;
    int m_offset;
};

// A buffer reader that allows reading char by char.
class BufferCharReader {
public:
    // Constructs a char reader attached to given buffer.
    BufferCharReader (Buffer const& buffer);

    // Returns the next char from the attached buffer.
    char get ();

    // Returns `true` if there is no more data to read.
    bool eob () const;

private:
    char const* const m_buffer;
    int const m_size;
    int m_pos;
};

// A slice of buffer data interpreted with char resolution.
class BufferCharSlice {
public:
    // Constructs a new slice of given length, starting at char with index
    // `begin` and ending. Providing `length <= 0` results in an empty slice.
    BufferCharSlice (Buffer const& buffer, int begin, int length);

private:
    char const* const m_begin;
    int const m_length;

    friend class BufferCharWriter;
};

// A buffer writer that allows writing with char resolution.
class BufferCharWriter {
public:
    // Constructs a char writer attached to given buffer.
    BufferCharWriter (Buffer& buffer);

    // Appends a char to the buffer.
    void put (char data);

    // Appends a string to the buffer.
    void put (string const& data);

    // Appends a buffer slice to the buffer. The slice may come from a different
    // buffer or it may come from the same buffer.
    void put (BufferCharSlice const& slice);

private:
    std::vector<word>& m_buffer;
    int& m_size;
    int m_pos;
};

inline Buffer::Buffer () :
    m_buffer(1, NULL_WORD),
    m_size(0)
{
    // Do nothing.
}

inline Buffer::Buffer (Buffer&& buffer) :
    m_size(buffer.m_size)
{
    swap(m_buffer, buffer.m_buffer);
}

inline int Buffer::size () const {
    return m_size;
}

inline bool Buffer::operator == (Buffer const& buffer) const {
    return m_size == buffer.m_size && m_buffer == buffer.m_buffer;
}

inline BufferBitReader::BufferBitReader(Buffer const& buffer) :
    m_buffer(buffer.m_buffer.data()),
    m_left(buffer.m_size),
    m_pos(0),
    m_offset(WORD_LENGTH)
{
    // Do nothing.
}

inline bool BufferBitReader::eob () const {
    return m_left <= 0;
}

inline BufferBitWriter::BufferBitWriter(Buffer& buffer) :
    m_buffer(buffer.m_buffer),
    m_size(buffer.m_size),
    m_pos(buffer.m_size / WORD_LENGTH),
    m_offset(WORD_LENGTH - buffer.m_size % WORD_LENGTH)
{
    // Do nothing.
}

inline BufferCharReader::BufferCharReader (Buffer const& buffer) :
    m_buffer(reinterpret_cast<char const*>(buffer.m_buffer.data())),
    m_size(buffer.m_size / CHAR_LENGTH),
    m_pos(0)
{
    // Do nothing
}

inline char BufferCharReader::get () {
    assert(m_pos < m_size);
    return m_buffer[m_pos++];
}

inline bool BufferCharReader::eob () const {
    return m_pos >= m_size;
}

inline BufferCharSlice::BufferCharSlice (
    Buffer const& buffer,
    int begin,
    int length
) :
    m_begin(reinterpret_cast<char const*>(buffer.m_buffer.data()) + begin),
    m_length(max(0, length))
{
    // Permit only proper ranges.
    assert(begin < buffer.size() / CHAR_LENGTH || m_length == 0);
    assert(begin + m_length <= buffer.size() / CHAR_LENGTH);
}

inline BufferCharWriter::BufferCharWriter (Buffer& buffer) :
    m_buffer(buffer.m_buffer),
    m_size(buffer.m_size),
    m_pos(buffer.m_size / CHAR_LENGTH)
{
    // Permit only properly aligned buffers.
    assert(buffer.m_size % CHAR_LENGTH == 0);
}

#endif // BUFFER_HPP
