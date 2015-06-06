#include "prefix.h"

#include "../src/lzw.h"
#include "../src/smru_dict.h"

class SmruLzwTest : public testing::Test {
protected:
    Lzw<SmruDict> lzw;
    Buffer input;
    Buffer output;

    SmruLzwTest ();
};

SmruLzwTest::SmruLzwTest () :
    lzw(3)
{
    // Real limit is `3 + 256`.
    int const cwbits = 9;

    BufferCharWriter iwriter(input);
    iwriter.put("aababbacabac");

    BufferBitWriter owriter(output);
    owriter.put(1, cwbits);
    // (a  a)
    // 1: a*
    owriter.put(1, cwbits);
    // (a [a) b]
    // 2: a*, 1: aa
    owriter.put(2, cwbits);
    // (a [a)(b] a)
    // 3: b*, 2: ab, 1: aa
    owriter.put(256 + 2, cwbits);
    //  a [a (b][a) b  b]
    // 2: ab, 1: ab*, 3: ba
    owriter.put(256 + 3, cwbits);
    //  a [a (b] a) b (b  a  c)
    // 3: ba, 1: ba*, 2: ab
    owriter.put(3, cwbits);
    //  a  a (b  a) b (b  a [c) a]
    // 2: c*, 3: ba, 1: bac
    owriter.put(1, cwbits);
    //  a  a (b  a) b  b  a [c (a] b)
    // 1: a*, 2: ca, 3: ba
    owriter.put(256 + 2, cwbits);
    //  a  a (b  a) b  b  a  c (a [b) a c]
    // 3: ba, 2: ba*, 1: ab
    owriter.put(3, cwbits);
    //  a  a (b  a) b  b  a  c  a [b  a (c] $)
    // 1: c*, 3: ba, 2: bac
}

TEST_F (SmruLzwTest, Encoding) {
    ASSERT_EQ(output, lzw.encode(input));
}

TEST_F (SmruLzwTest, Decoding) {
    ASSERT_EQ(input, lzw.decode(output));
}
