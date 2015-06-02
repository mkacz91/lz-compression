#include "prefix.hpp"

#include "../src/lz78.hpp"
#include "../src/trie_encode_dict.hpp"

TEST(Lz78, InfiniteEncodesProperly) {
    Lz78<TrieEncodeDict> lz78;

    Buffer input;
    BufferCharWriter iwriter(input);
    iwriter.put("aababbacabca");

    Buffer expected_output;
    BufferBitWriter eowriter(expected_output);
    eowriter.put( 0 , WORD_LENGTH);
    eowriter.put('a', CHAR_LENGTH); // a|a b a b b a c a b c a
    eowriter.put( 1 , WORD_LENGTH);
    eowriter.put('b', CHAR_LENGTH); // a|a b|a b b a c a b c a
    eowriter.put( 2 , WORD_LENGTH);
    eowriter.put('b', CHAR_LENGTH); // a|a b|a b b|a c a b c a
    eowriter.put( 1 , WORD_LENGTH);
    eowriter.put('c', CHAR_LENGTH); // a|a b|a b b|a c|a b c a
    eowriter.put( 2 , WORD_LENGTH);
    eowriter.put('c', CHAR_LENGTH); // a|a b|a b b|a c|a b c|a
    eowriter.put( 1 , WORD_LENGTH); // a|a b|a b b|a c|a b c|a.

    Buffer output = lz78.encode(input);

    ASSERT_EQ(expected_output, output);
}
