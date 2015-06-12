#include "prefix.h"

#include "../src/mra_dict.h"

TEST (MraDictTest, Encoding) {
    Buffer input;
    BufferCharWriter writer(input);
    writer.put("aabccdacdacabd");
    Mra::EncodeDict d(input, 3, false);

    ASSERT_EQ(Match(0, 0, 'a'), d.try_char()); // a
    // a|
    // 1
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|a
    // 1
    ASSERT_EQ(Match(1, 1, 'b'), d.try_char()); // b
    // a|a b|
    // 1  2
    ASSERT_EQ(Match(0, 0, 'c'), d.try_char()); // c
    // a|a b|c|
    // 1  2  3
    ASSERT_FALSE(d.try_char().is_maximal());   // c
    // a|a b|c|c
    // 1  2  3
    ASSERT_EQ(Match(3, 1, 'd'), d.try_char()); // d
    // *|a b|c|c d|
    //    2  3  1
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // *|a b|c|c d|a
    //    2  3  1
    ASSERT_EQ(Match(0, 0, 'a'), d.try_char()); // c
    // *|* *|c|c d|a|c
    //       3  1  2
    d.put_back(1);
    // *|* *|c|c d|a|
    //       3  1  2
    ASSERT_FALSE(d.try_char().is_maximal());   // c
    // *|* *|c|c d|a|c
    //       3  1  2
    ASSERT_FALSE(d.try_char().is_maximal());   // d
    // *|* *|c|c d|a|c d
    //       3  1  2
    ASSERT_EQ(Match(1, 2, 'a'), d.try_char()); // a
    // *|* *|*|c d|a|c d a|
    //          1  2   3
    ASSERT_FALSE(d.try_char().is_maximal());   // c
    // *|* *|*|c d|a|c d a|c
    //          1  2   3
    ASSERT_EQ(Match(0, 0, 'c'), d.try_char()); // a
    // *|* *|*|* *|a|c d a|c|a
    //             2   3   1
    d.put_back(1);
    // *|* *|*|* *|a|c d a|c|
    //             2   3   1
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // *|* *|*|* *|a|c d a|c|a
    //             2   3   1
    ASSERT_EQ(Match(2, 1, 'b'), d.try_char()); // b
    // *|* *|*|* *|*|c d a|c|a b|
    //                 3   1  2
    ASSERT_EQ(Match(0, 0, 'd'), d.try_char()); // d
    // *|* *|*|* *|*|* * *|c|a b|d
    //                     1  2  3
}

TEST (MraDictTest, Decoding) {
    Mra::DecodeDict d(3, false);

    d.add_extension(0, 1); // 0->1
    d.add_extension(0, 2); // 0->1, 0->2
    d.add_extension(1, 3); // 0->1, 0->2, 1->3

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 1), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(3, 2), d.codeword(3));

    d.add_extension(2, 4); // 2->4, 0->2, 1->3

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(4, 2), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(3, 2), d.codeword(3));
}
