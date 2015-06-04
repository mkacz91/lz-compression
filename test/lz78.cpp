#include "prefix.hpp"

#include "../src/lz78.hpp"
#include "../src/smru_dict.hpp"

class Lz78WithMruDictTest : public testing::Test {
protected:
    Lz78<SmruDict> lz78;
    Buffer input;
    Buffer output;

    Lz78WithMruDictTest ();

    void SetUp () override;
};

Lz78WithMruDictTest::Lz78WithMruDictTest () :
    lz78(3)
{
    // Do nothing.
}

void Lz78WithMruDictTest::SetUp () {
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

TEST_F (Lz78WithMruDictTest, Encoding) {
    ASSERT_EQ(output, lz78.encode(input));
}

TEST_F (Lz78WithMruDictTest, Decoding) {
    ASSERT_EQ(input, lz78.decode(output));
}
