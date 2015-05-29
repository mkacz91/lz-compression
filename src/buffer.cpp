#include "buffer.hpp"

word BufferBitReader::get (int bit_cnt) {
    assert(bit_cnt >= 0);
    assert(bit_cnt <= WORD_LENGTH);
    assert(bit_cnt <= m_left);

    m_offset -= bit_cnt;
    word result = rshift(m_buffer[m_pos], m_offset);
    if (m_offset <= 0) {
        m_offset += WORD_LENGTH;
        ++m_pos;
        result |= rshift(m_buffer[m_pos], m_offset);
    }
    m_left -= bit_cnt;
    return result & ~lshift(ONES_MASK, bit_cnt);
}

void BufferBitWriter::put (word data, int bit_cnt) {
    assert(bit_cnt >= 0);
    assert(bit_cnt <= WORD_LENGTH);
    assert((data & ~lshift(ONES_MASK, bit_cnt)) == data);

    m_offset -= bit_cnt;
    m_buffer[m_pos] |= lshift(data, m_offset);
    if (m_offset <= 0) {
        m_offset += WORD_LENGTH;
        ++m_pos;
        m_buffer.push_back(lshift(data, m_offset));
    }
    m_size += bit_cnt;
}

void BufferCharWriter::put (char data) {
    reinterpret_cast<char*>(m_buffer.data())[m_pos] = data;
    m_size += CHAR_LENGTH;
    ++m_pos;
    if (m_buffer.size() <= m_pos * CHAR_LENGTH / WORD_LENGTH)
        m_buffer.push_back(NULL_WORD);
}

void BufferCharWriter::put (string const& data) {
    for (char c : data)
        put(c);
}
