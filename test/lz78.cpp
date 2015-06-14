#include "prefix.h"

#include "../src/lz78.h"
#include "../src/smru_dict.h"

class SmruLz78Test : public testing::Test {
protected:
    Lz78<Smru> lz78;
    Buffer input;
    Buffer output;

    SmruLz78Test ();
};

SmruLz78Test::SmruLz78Test () :
    lz78(3)
{
    int const codeword_no_length = 2;

    BufferCharWriter iwriter(input);
    iwriter.put("aababbacabca");

    BufferBitWriter owriter(output);
    owriter.put( 0 , codeword_no_length);
    owriter.put('a', CHAR_BITS       ); // a|
    owriter.put( 1 , codeword_no_length);
    owriter.put('b', CHAR_BITS       ); // a|a b|
    owriter.put( 2 , codeword_no_length);
    owriter.put('b', CHAR_BITS       ); // a|a b|a b b|
    owriter.put( 1 , codeword_no_length);
    owriter.put('c', CHAR_BITS       ); // a|a b|* * *|a c|
    owriter.put( 2 , codeword_no_length);
    owriter.put('c', CHAR_BITS       ); // a|a b|* * *|* *|a b c|a
    owriter.put( 1 , codeword_no_length); // a|a b|* * *|* *|a b c|a
}

TEST_F (SmruLz78Test, Encoding) {
    ASSERT_EQ(output, lz78.encode(input));
}

TEST_F (SmruLz78Test, Decoding) {
    ASSERT_EQ(input, lz78.decode(output));
}
