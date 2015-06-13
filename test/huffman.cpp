#include "prefix.h"

#include "../src/huffman.h"

TEST (HuffmanTest, SingleChar) {
    Buffer input;
    BufferCharWriter writer(input);
    writer.put("x");
    cout << input << endl;
    ASSERT_EQ(input, Huffman::decode(Huffman::encode(input)));
}

TEST (HuffmanTest, LoremIpsum) {
    Buffer input;
    BufferCharWriter writer(input);
    writer.put(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec "
        "interdum cursus venenatis. Proin eget fringilla nulla, ut sagittis "
        "sem. Pellentesque habitant morbi tristique senectus et netus et "
        "malesuada fames ac turpis egestas. Nunc ultrices erat sit amet leo "
        "accumsan congue. Duis faucibus justo felis, vel pulvinar enim tempus "
        "condimentum. Sed aliquam placerat nisi, vitae posuere eros finibus "
        "sed. Aenean vestibulum et metus eu congue. Pellentesque nec velit ut "
        "purus suscipit dapibus. Interdum et malesuada fames ac ante ipsum "
        "primis in faucibus. Nam eu rhoncus quam. Suspendisse consectetur "
        "neque turpis, vitae pellentesque quam vulputate sed."
    );
    ASSERT_EQ(input, Huffman::decode(Huffman::encode(input)));
}

TEST (HuffmanTest, Uneven) {
    Buffer input;
    BufferBitWriter writer(input);
    writer.put(0x12345678, 32);
    writer.put(0x0009ABCD, 20);
    ASSERT_EQ(input, Huffman::decode(Huffman::encode(input)));
}
