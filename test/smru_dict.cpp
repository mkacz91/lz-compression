#include "prefix.hpp"

#include "../src/smru_dict.cpp"

TEST (SmruDictTest, Encoding) {
    SmruEncodeDict d(3);
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

TEST (SmruDictTest, Decoding) {
    SmruDecodeDict d(3);

    d.add_extension(0, 1); // 1: 1
    d.add_extension(0, 2); // 2: 2, 1: 1
    d.add_extension(1, 3); // 1: 1, 3: 3, 2: 2

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 1), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(3, 2), d.codeword(3));

    d.add_extension(2, 4); // 1: 1, 2: 2, 3: 4

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 1), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(4, 2), d.codeword(3));

    d.add_extension(3, 5); // 2: 2, 3: 4, 1: 5

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(5, 3), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(4, 2), d.codeword(3));

    d.add_extension(1, 6); // 2: 2, 3: 4, 1: 5 (rejected)

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(5, 3), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(4, 2), d.codeword(3));
}
