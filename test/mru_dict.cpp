#include "prefix.hpp"

#include "../src/mru_encode_dict.cpp"

TEST(MruDictTest, Encoding) {
    MruEncodeDict d(3);
    // empty queue
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ( 0, d.try_char('a')); // a|
    // 1: a
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('a')); // a|a
    // 1: a
    ASSERT_EQ( 1, d.peek_codeword_no());
    EXPECT_EQ( 1, d.try_char('b')); // a|a b|
    // 1: a, 2: ab
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ( 0, d.try_char('c')); // a|a b|c|
    // 3: c, 1: a, 2: ab
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('a')); // a|a b|c|a
    // 3: c, 1: a, 2: ab
    ASSERT_EQ( 1, d.peek_codeword_no());
    EXPECT_EQ( 1, d.try_char('c')); // a|* *|c|a c|
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('c')); // a|* *|c|a c|c
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 3, d.peek_codeword_no());
    EXPECT_EQ( 3, d.try_char('a')); // a|* *|c|* *|c a|
    // 3: c, 2: ca, 1: a
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('a')); // a|* *|c|* *|c a|a
    // 3: c, 2: ca, 1: a
    ASSERT_EQ( 1, d.peek_codeword_no());
    EXPECT_EQ( 1, d.try_char('c')); // a|* *|c|* *|* *|a c|
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('a')); // a|* *|*|* *|* *|a c|a
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 1, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('c')); // a|* *|*|* *|* *|a c|a c
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 2, d.peek_codeword_no());
    EXPECT_EQ( 2, d.try_char('b')); // a|* *|*|* *|* *|a c|a c b|
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('a')); // a|* *|*|* *|* *|a c|a c b|a
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 1, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('c')); // a|* *|*|* *|* *|a c|a c b|a c
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 2, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('b')); // a|* *|*|* *|* *|a c|a c b|a c b
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 3, d.peek_codeword_no());
    EXPECT_EQ( 3, d.try_char('d')); // a|* *|*|* *|* *|a c|a c b|* * * *|
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 0, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('a')); // a|* *|*|* *|* *|a c|a c b|* * * *|a
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 1, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('c')); // a|* *|*|* *|* *|a c|a c b|* * * *|a c
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 2, d.peek_codeword_no());
    EXPECT_EQ(-1, d.try_char('b')); // a|* *|*|* *|* *|a c|a c b|* * * *|a c b
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 3, d.peek_codeword_no());
    EXPECT_EQ( 3, d.try_char('d')); // a|* *|*|* *|* *|a c|a c b|* * * *|* * * *
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 0, d.peek_codeword_no());
}
