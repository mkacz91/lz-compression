#include "prefix.hpp"

#include "../src/buffer.hpp"

TEST(Buffer, BitIOWorksCorrectly) {
    Buffer buffer;

    BufferBitWriter writer1(buffer);
    writer1.put(0x00000001,  1);
    ASSERT_EQ( 1, buffer.size());
    writer1.put(0x00000003,  3);
    ASSERT_EQ( 4, buffer.size());
    writer1.put(0x0000FFFF, 16);
    ASSERT_EQ(20, buffer.size());
    writer1.put(0x1234ABCD, 32);
    ASSERT_EQ(52, buffer.size());

    BufferBitWriter writer2(buffer);
    writer2.put(0x0F0F0F0F, 32);
    ASSERT_EQ(84, buffer.size());
    writer2.put(0x00000001,  1);
    ASSERT_EQ(85, buffer.size());

    BufferBitReader reader(buffer);
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ(0x00000002, reader.get( 2));
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ(0x00000007, reader.get( 3));
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ(0x00007FFF, reader.get(15));
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ(0x1234ABCD, reader.get(32));
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ(0x07878787, reader.get(31));
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ(0x00000003, reader.get( 2));
    ASSERT_TRUE(reader.eob());
}

TEST(Buffer, CharIOWorksCorrectly) {
    Buffer buffer;

    BufferCharWriter writer1(buffer);
    writer1.put('a');
    ASSERT_EQ(1 * CHAR_LENGTH, buffer.size());
    writer1.put('b');
    ASSERT_EQ(2 * CHAR_LENGTH, buffer.size());
    writer1.put("cd");
    ASSERT_EQ(4 * CHAR_LENGTH, buffer.size());

    BufferCharWriter writer2(buffer);
    writer2.put('e');
    ASSERT_EQ(5 * CHAR_LENGTH, buffer.size());

    BufferCharReader reader(buffer);
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ('a', reader.get());
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ('b', reader.get());
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ('c', reader.get());
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ('d', reader.get());
    ASSERT_FALSE(reader.eob());
    ASSERT_EQ('e', reader.get());
    ASSERT_TRUE(reader.eob());
}
