#include "prefix.hpp"

#include "../src/bit_buffer.hpp"

TEST(BitBuffer, CorrectlyAdjustsSize) {
    BitBuffer buffer;
    EXPECT_EQ(0, buffer.size());
    buffer.put(0, 1);
    EXPECT_EQ(1, buffer.size());
    buffer.get(1);
    EXPECT_EQ(0, buffer.size());
    buffer.put(0, 5);
    EXPECT_EQ(5, buffer.size());
    buffer.put(0, WORD_LENGTH);
    EXPECT_EQ(WORD_LENGTH + 5, buffer.size());
    buffer.get(6);
    EXPECT_EQ(WORD_LENGTH - 1, buffer.size());
}
