#include "buffer.h"

Buffer::Buffer () :
    m_data(new word[1]),
    m_capacity(1),
    m_open_word_cnt(0),
    m_size(0)
{
    push_back(NULL_WORD);
}

Buffer::Buffer (Buffer&& buffer) :
    m_capacity(buffer.m_capacity),
    m_open_word_cnt(buffer.m_open_word_cnt),
    m_size(buffer.m_size)
{
    swap(m_data, buffer.m_data);
}

Buffer::~Buffer () {
    delete[] m_data;
}

inline void Buffer::push_back (word data) {
    delete[] adjust_capacity(m_open_word_cnt + 1);
    m_data[m_open_word_cnt] = data;
    ++m_open_word_cnt;
}

inline word* Buffer::adjust_capacity (int word_cnt) {
    if (word_cnt <= m_capacity)
        return nullptr;
    word* old_data = m_data;
    int old_capacity = m_capacity;
    while (m_capacity < word_cnt)
        m_capacity *= 2;
    m_data = new word[m_capacity];
    copy(old_data, old_data + old_capacity, m_data);
    return old_data;
}

bool Buffer::operator == (Buffer const& buffer) const {
    if (m_size != buffer.m_size)
        return false;
    for (int i = 0; i < m_open_word_cnt; ++i)
        if (m_data[i] != buffer.m_data[i])
            return false;
    return true;
}

std::ostream& operator << (std::ostream& ostr, Buffer const& buffer) {
    // This is intended only for debugging purposes so it doesn't have to be
    // effective.
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

BufferBitReader::BufferBitReader(Buffer const& buffer) :
    m_data(buffer.m_data),
    m_left(buffer.m_size),
    m_pos(0),
    m_offset(WORD_LENGTH)
{
    // Do nothing.
}

word BufferBitReader::get (int bit_cnt) {
    assert(bit_cnt >= 0);
    assert(bit_cnt <= WORD_LENGTH);
    assert(bit_cnt <= m_left);

    m_offset -= bit_cnt;
    word result = rshift(m_data[m_pos], m_offset);
    if (m_offset <= 0) {
        m_offset += WORD_LENGTH;
        ++m_pos;
        result |= rshift(m_data[m_pos], m_offset);
    }
    m_left -= bit_cnt;
    return result & ~lshift(ONES_MASK, bit_cnt);
}

BufferBitWriter::BufferBitWriter(Buffer& buffer) :
    m_buffer(buffer),
    m_pos(buffer.m_size / WORD_LENGTH),
    m_offset(WORD_LENGTH - buffer.m_size % WORD_LENGTH)
{
    // Do nothing.
}

void BufferBitWriter::put (word data, int bit_cnt) {
    assert(bit_cnt >= 0);
    assert(bit_cnt <= WORD_LENGTH);
    assert((data & ~lshift(ONES_MASK, bit_cnt)) == data);

    m_offset -= bit_cnt;
    m_buffer.m_data[m_pos] |= lshift(data, m_offset);
    if (m_offset <= 0) {
        m_offset += WORD_LENGTH;
        ++m_pos;
        m_buffer.push_back(lshift(data, m_offset));
    }
    m_buffer.m_size += bit_cnt;
}

BufferCharReader::BufferCharReader (Buffer const& buffer) :
    m_data(reinterpret_cast<char const*>(buffer.m_data)),
    m_char_cnt(buffer.m_size / CHAR_LENGTH),
    m_pos(0)
{
    // Do nothing
}

BufferCharWriter::BufferCharWriter (Buffer& buffer) :
    m_buffer(buffer),
    m_pos(buffer.m_size / CHAR_LENGTH)
{
    // Permit only properly aligned buffers.
    assert(buffer.m_size % CHAR_LENGTH == 0);
}

void BufferCharWriter::put (char data) {
    reinterpret_cast<char*>(m_buffer.m_data)[m_pos] = data;
    m_buffer.m_size += CHAR_LENGTH;
    ++m_pos;
    if (m_buffer.m_open_word_cnt <= m_pos * CHAR_LENGTH / WORD_LENGTH)
        m_buffer.push_back(NULL_WORD);
}

void BufferCharWriter::put (string const& data) {
    // This method is intended for testing, so it doesn't have to be efficient.
    for (char c : data)
        put(c);
}

void BufferCharWriter::put (BufferCharSlice const& slice) {
    int new_pos = m_pos + slice.m_length;
    int new_open_word_cnt = new_pos * CHAR_LENGTH / WORD_LENGTH + 1;
    word* const old_data = m_buffer.adjust_capacity(new_open_word_cnt);
    if (m_buffer.m_open_word_cnt < new_open_word_cnt) {
        // If new words are opened. The last one would end up with undefined
        // least significant bits if not initialized.
        m_buffer.m_data[new_open_word_cnt - 1] = NULL_WORD;
        m_buffer.m_open_word_cnt = new_open_word_cnt;
    }
    copy(
        slice.m_begin,
        slice.m_begin + slice.m_length,
        reinterpret_cast<char*>(m_buffer.m_data) + m_pos
    );
    // The deletion of `old_data` is delayed until now, because `slice` may
    // come from `m_buffer` itself.
    delete[] old_data;
    m_buffer.m_size += slice.m_length * CHAR_LENGTH;
    m_pos = new_pos;
}
