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

    // Returns number of bits stored in the buffer.
    int size () const;

private:
    std::vector<word> m_buffer;
    int m_size;

    friend class BufferBitReader;
    friend class BufferBitWriter;
    friend class BufferCharReader;
    friend class BufferCharWriter;
};

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
    BufferBitWriter (Buffer& buffer);

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
    BufferCharReader (Buffer const& buffer);

    char get ();

    bool eob () const;

private:
    char const* const m_buffer;
    int const m_size;
    int m_pos;
};

// A buffer writer that allows writing with char resolution.
class BufferCharWriter {
public:
    BufferCharWriter (Buffer& buffer);

    void put (char data);

    void put (string const& data);

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

inline int Buffer::size () const {
    return m_size;
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

inline BufferCharWriter::BufferCharWriter (Buffer& buffer) :
    m_buffer(buffer.m_buffer),
    m_size(buffer.m_size),
    m_pos(buffer.m_size / CHAR_LENGTH)
{
    // Permit only properly aligned buffers.
    assert(buffer.m_size % CHAR_LENGTH == 0);
}

#endif // BUFFER_HPP
