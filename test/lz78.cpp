#include "prefix.h"

#include "../src/lz78.h"
#include "../src/smru_dict.h"

class SmruLz78Test : public testing::Test {
protected:
    Lz78<SmruDict> lz78;
    Buffer input;
    Buffer output;

    SmruLz78Test ();

    virtual void SetUp ();
};

SmruLz78Test::SmruLz78Test () :
    lz78(3)
{
    // Do nothing.
}

void SmruLz78Test::SetUp () {
    int const codeword_no_length = 2;

    BufferCharWriter iwriter(input);
    iwriter.put("aababbacabca");

    BufferBitWriter owriter(output);
    owriter.put( 0 , codeword_no_length);
    owriter.put('a', CHAR_LENGTH       ); // a|
    owriter.put( 1 , codeword_no_length);
    owriter.put('b', CHAR_LENGTH       ); // a|a b|
    owriter.put( 2 , codeword_no_length);
    owriter.put('b', CHAR_LENGTH       ); // a|a b|a b b|
    owriter.put( 1 , codeword_no_length);
    owriter.put('c', CHAR_LENGTH       ); // a|a b|* * *|a c|
    owriter.put( 2 , codeword_no_length);
    owriter.put('c', CHAR_LENGTH       ); // a|a b|* * *|* *|a b c|a
    owriter.put( 1 , codeword_no_length); // a|a b|* * *|* *|a b c|a
}

TEST_F (SmruLz78Test, Encoding) {
    ASSERT_EQ(output, lz78.encode(input));
}

TEST_F (SmruLz78Test, Decoding) {
    ASSERT_EQ(input, lz78.decode(output));
}

TEST_F (SmruLz78Test, SomeMoreEncodingAndDecoding) {
    Lz78<SmruDict> lz78(50);

    Buffer input1;
    BufferCharWriter writer1(input1);
    writer1.put(
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
    EXPECT_EQ(input1, lz78.decode(lz78.encode(input1)));

    Buffer input2;
    BufferCharWriter writer2(input2);
    writer2.put('x');
    EXPECT_EQ(input2, lz78.decode(lz78.encode(input2)));
}
