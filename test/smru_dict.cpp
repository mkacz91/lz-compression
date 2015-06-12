#include "prefix.h"

#include "../src/smru_dict.cpp"

TEST (SmruDictTest, Encoding) {
    Buffer input;
    BufferCharWriter writer(input);
    writer.put("aabcacabcabcabcdabcda");
    Smru::EncodeDict d(input, 3, false);

    ASSERT_EQ(Match(0, 0, 'a'), d.try_char()); // a
    // a|
    // 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|a
    // 1: a
    ASSERT_EQ(Match(1, 1, 'b'), d.try_char()); // b
    // a|a b|
    // 2: ab, 1: a
    ASSERT_EQ(Match(0, 0, 'c'), d.try_char()); // c
    // a|a b|c|
    // 2: ab, 1: a, 3: c
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|a b|c|a
    // 2: ab, 1: a, 3: c
    ASSERT_EQ(Match(1, 1, 'c'), d.try_char()); // c
    // a|* *|c|a c|
    // 3: c, 2: ac, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|* *|c|a c|a
    // 3: c, 2: ac, 1: a
    ASSERT_EQ(Match(1, 1, 'b'), d.try_char()); // b
    // a|* *|*|a c|a b|
    // 2: ac, 3: ab, 1: a
    ASSERT_EQ(Match(0, 0, 'c'), d.try_char()); // c
    // a|* *|*|* *|a b|c|
    // 3: ab, 1: a, 2: c
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|* *|*|* *|a b|c|a
    // 3: ab, 1: a, 2: c
    ASSERT_FALSE(d.try_char().is_maximal());   // b
    // a|* *|*|* *|a b|c|a b
    // 3: ab, 1: a, 2: c
    ASSERT_EQ(Match(3, 2, 'c'), d.try_char()); // c
    // a|* *|*|* *|a b|*|a b c|
    // 2: abc, 3: ab, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|* *|*|* *|a b|*|a b c|a
    // 2: abc, 3: ab, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // b
    // a|* *|*|* *|a b|*|a b c|a b
    // 2: abc, 3: ab, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // c
    // a|* *|*|* *|a b|*|a b c|a b c
    // 2: abc, 3: ab, 1: a
    ASSERT_EQ(Match(2, 3, 'd'), d.try_char()); // d
    // a|* *|*|* *|a b|*|a b c|* * * *|
    // 2: abc, 3: ab, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|* *|*|* *|a b|*|a b c|* * * *|a
    // 2: abc, 3: ab, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // b
    // a|* *|*|* *|a b|*|a b c|* * * *|a b
    // 2: abc, 3: ab, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // c
    // a|* *|*|* *|a b|*|a b c|* * * *|a b c
    // 2: abc, 3: ab, 1: a
    ASSERT_EQ(Match(2, 3, 'd'), d.try_char()); // d
    // a|* *|*|* *|a b|*|a b c|* * * *|* * * *|
    // 2: abc, 3: ab, 1: a
    ASSERT_FALSE(d.try_char().is_maximal());   // a
    // a|* *|*|* *|a b|*|a b c|* * * *|* * * *|a
    // 2: abc, 3: ab, 1: a
    Match match = d.fail_char();               // $
    EXPECT_EQ(1, match.codeword_no);
    EXPECT_EQ(1, match.length);
    // a|* *|*|* *|a b|*|a b c|* * * *|* * * *|a $|
    // 2: abc, 3: ab, 1: a
}

TEST (SmruDictTest, EncodingWithSingleCharCodewords) {
    Buffer input;
    BufferCharWriter writer(input);
    writer.put("ababccadaabc");
    Smru::EncodeDict d(input, 256 + 3, true);

    ASSERT_FALSE(d.try_char().is_maximal());              // a
    // a
    //
    ASSERT_EQ(Match(int('a') + 1, 1, 'b'), d.try_char()); // b
    // a b|
    // 1: ab
    ASSERT_FALSE(d.try_char().is_maximal());              // a
    // a b|a
    // 1: ab
    ASSERT_FALSE(d.try_char().is_maximal());              // b
    // a b|a b
    // 1: ab
    ASSERT_EQ(Match(CHAR_CNT + 1, 2, 'c'), d.try_char()); // c
    // a b|a b c|
    // 2: abc, 1: ab
    ASSERT_FALSE(d.try_char().is_maximal());              // c
    // a b|a b c|c
    // 2: abc, 1: ab
    ASSERT_EQ(Match(int('c') + 1, 1, 'a'), d.try_char()); // a
    // a b|a b c|c a|
    // 2: abc, 1: ab, 3: ca
    ASSERT_FALSE(d.try_char().is_maximal());              // d
    // a b|a b c|c a|d
    // 2: abc, 1: ab, 3: ca
    ASSERT_EQ(Match(int('d') + 1, 1, 'a'), d.try_char()); // a
    // a b|* * *|c a|d a|
    // 1: ab, 3: ca, 2: da
    ASSERT_FALSE(d.try_char().is_maximal());              // a
    // a b|* * *|c a|d a|a
    // 1: ab, 3: ca, 2: da
    ASSERT_FALSE(d.try_char().is_maximal());              // b
    // a b|* * *|c a|d a|a b
    // 1: ab, 3: ca, 2: da
    ASSERT_EQ(Match(CHAR_CNT + 1, 2, 'c'), d.try_char()); // c
    // a b|* * *|* *|d a|a b c
    // 2: da, 3: abc, 1: ab
}

TEST (SmruDictTest, Decoding) {
    Smru::DecodeDict d(3, false);

    d.add_extension(0, 1); // 1: 1
    d.add_extension(0, 2); // 1: 1, 2: 2
    d.add_extension(1, 3); // 2: 2, 3: 3, 1: 1

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 1), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(3, 2), d.codeword(3));

    d.add_extension(2, 4); // 1: 1, 3: 4, 2: 2

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 1), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(4, 2), d.codeword(3));

    d.add_extension(3, 5); // 1: 5, 3: 4, 2: 2

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(5, 3), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(4, 2), d.codeword(3));

    d.add_extension(1, 6); // 1: 5, 3: 4, 2: 2 (rejected)

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(5, 3), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(4, 2), d.codeword(3));
}

TEST (SmruDictTest, DecodingWithSingleCharCodewords) {
    Smru::DecodeDict d(CHAR_CNT + 3, true);

    d.add_extension(int('a') + 1, 1); // 1: 1
    d.add_extension(int('b') + 1, 2); // 1: 1, 2: 2
    d.add_extension(CHAR_CNT + 1, 3); // 2: 2, 3: 2, 1: 1

    for (int a = 0; a < CHAR_CNT; ++a)
        ASSERT_EQ(1, d.codeword(a + 1).length);
    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 2), d.codeword(CHAR_CNT + 1));
    ASSERT_EQ(Codeword(2, 2), d.codeword(CHAR_CNT + 2));
    ASSERT_EQ(Codeword(3, 3), d.codeword(CHAR_CNT + 3));

    d.add_extension(CHAR_CNT + 2, 4); // 1: 1, 3: 4, 2: 2

    for (int a = 0; a < CHAR_CNT; ++a)
        ASSERT_EQ(1, d.codeword(a + 1).length);
    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 2), d.codeword(CHAR_CNT + 1));
    ASSERT_EQ(Codeword(2, 2), d.codeword(CHAR_CNT + 2));
    ASSERT_EQ(Codeword(4, 3), d.codeword(CHAR_CNT + 3));

    d.add_extension(CHAR_CNT + 3, 5); // 1: 5, 3: 4, 2: 2

    for (int a = 0; a < CHAR_CNT; ++a)
        ASSERT_EQ(1, d.codeword(a + 1).length);
    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(5, 4), d.codeword(CHAR_CNT + 1));
    ASSERT_EQ(Codeword(2, 2), d.codeword(CHAR_CNT + 2));
    ASSERT_EQ(Codeword(4, 3), d.codeword(CHAR_CNT + 3));

    d.add_extension(CHAR_CNT + 1, 6); // 1: 5, 3: 4, 2: 2 (rejected)

    for (int a = 0; a < CHAR_CNT; ++a)
        ASSERT_EQ(1, d.codeword(a + 1).length);
    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(5, 4), d.codeword(CHAR_CNT + 1));
    ASSERT_EQ(Codeword(2, 2), d.codeword(CHAR_CNT + 2));
    ASSERT_EQ(Codeword(4, 3), d.codeword(CHAR_CNT + 3));
}
