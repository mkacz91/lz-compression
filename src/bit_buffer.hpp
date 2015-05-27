#ifndef BIT_BUFFER_HPP
#define BIT_BUFFER_HPP

#include "prefix.hpp"
#include <vector>

class BitBuffer {
public:
    BitBuffer ();

    int size () const { return m_size; }

    word get (int bit_cnt);
    void put (word data, int bit_cnt);

private:
    std::vector<word> m_buffer;
    int m_size;
    int m_get_pos;
    int m_get_offset;
    int m_put_pos;
    int m_put_offset;
};

inline BitBuffer::BitBuffer ()
    : m_buffer(1, NULL_WORD)
    , m_size(0)
    , m_get_pos(0)
    , m_get_offset(WORD_LENGTH)
    , m_put_pos(0)
    , m_put_offset(WORD_LENGTH)
{
    // Do nothing.
}

#endif // BIT_STREAM_HPP
