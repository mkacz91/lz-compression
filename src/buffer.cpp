#include "buffer.hpp"

std::ostream& operator << (std::ostream& ostr, Buffer const& buffer) {
    BufferBitReader reader(buffer);
    int pos = 0;
    while (!reader.eob()) {
        ostr << (reader.get(1) == NULL_WORD ? '0' : '1');
        if (++pos == 8) {
            ostr << ' ';
            pos = 0;
        }
    }
    return ostr;
}

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

void BufferCharWriter::put (BufferCharSlice const& slice) {
    int new_pos = m_pos + slice.m_length;
    m_buffer.resize(new_pos * CHAR_LENGTH / WORD_LENGTH + 1);
    copy(
        slice.m_begin,
        slice.m_begin + slice.m_length,
        reinterpret_cast<char*>(m_buffer.data()) + m_pos
    );
    m_size += slice.m_length * CHAR_LENGTH;
    m_pos = new_pos;
}
