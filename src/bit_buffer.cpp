#include "bit_buffer.hpp"

word BitBuffer::get (int bit_cnt) {
    assert(bit_cnt <= WORD_LENGTH);
    assert(bit_cnt <= m_size);

    m_get_offset -= bit_cnt;
    word result = (m_buffer[m_get_pos] >> m_get_offset);
    if (m_get_offset <= 0) {
        m_get_offset += WORD_LENGTH;
        ++m_get_pos;
        result |= (m_buffer[m_get_pos] >> m_get_offset);
    }
    m_size -= bit_cnt;
    return result & ~(ONES_MASK << bit_cnt);
}

void BitBuffer::put (word data, int bit_cnt) {
    assert(bit_cnt <= WORD_LENGTH);
    assert((data & ~(ONES_MASK << bit_cnt)) == data);

    m_put_offset -= bit_cnt;
    m_buffer[m_put_pos] |= (data << m_put_offset);
    if (m_put_offset <= 0) {
        m_put_offset += WORD_LENGTH;
        ++m_put_pos;
        m_buffer.push_back(data << m_put_offset);
    }
    m_size += bit_cnt;
}
