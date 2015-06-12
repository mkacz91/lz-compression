#include "prefix.h"

#include "../src/lzw.h"
#include "../src/smru_dict.h"

class SmruLzwTest : public testing::Test {
protected:
    Lzw<Smru> lzw;
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
    owriter.put(1 + 'a', cwbits);
    // (a  a)
    // 1: a*
    owriter.put(1 + 'a', cwbits);
    // (a [a) b]
    // 1: aa, 2: a*
    owriter.put(1 + 'b', cwbits);
    // (a [a)(b] a)
    // 1: aa, 2: ab, 3: b*
    owriter.put(256 + 2, cwbits);
    //  a [a (b][a) b  b]
    // 3: ba, 1: ab*, 2: ab
    owriter.put(256 + 3, cwbits);
    //  a [a (b] a) b (b  a  c)
    // 2: ab, 1: ba*, 3: ba
    owriter.put(1 + 'c', cwbits);
    //  a  a (b  a) b (b  a [c) a]
    // 1: bac, 3: ba, 2: c*
    owriter.put(1 + 'a', cwbits);
    //  a  a (b  a) b  b  a [c (a] b)
    // 3: ba, 2: ca, 1: a*
    owriter.put(256 + 3, cwbits);
    //  a  a (b  a) b  b  a  c (a [b) a c]
    // 1: ab, 2: ba*, 3: ba
    owriter.put(1 + 'c', cwbits);
    //  a  a (b  a) b  b  a  c  a [b  a (c] $)
    // 2: bac, 3: ba, 1: c*
}

TEST_F (SmruLzwTest, Encoding) {
    ASSERT_EQ(output, lzw.encode(input));
}

TEST_F (SmruLzwTest, Decoding) {
    ASSERT_EQ(input, lzw.decode(output));
}
