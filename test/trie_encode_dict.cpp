#include "prefix.hpp"

#include "../src/trie_encode_dict.cpp"

TEST(TrieEncodeDict, UnlimitedFactorsProperly) {
    TrieEncodeDict dict;
    ASSERT_EQ( 0, dict.try_char('a')); // a|
    ASSERT_EQ(-1, dict.try_char('a')); // a|a
    ASSERT_EQ( 1, dict.try_char('b')); // a|a b|
    ASSERT_EQ(-1, dict.try_char('a')); // a|a b|a
    ASSERT_EQ(-1, dict.try_char('b')); // a|a b|a b
    ASSERT_EQ( 2, dict.try_char('b')); // a|a b|a b b|
    ASSERT_EQ(-1, dict.try_char('a')); // a|a b|a b b|a
    ASSERT_EQ( 1, dict.try_char('c')); // a|a b|a b b|a c|
    ASSERT_EQ(-1, dict.try_char('a')); // a|a b|a b b|a c|a
    ASSERT_EQ(-1, dict.try_char('b')); // a|a b|a b b|a c|a b
    ASSERT_EQ( 2, dict.try_char('c')); // a|a b|a b b|a c|a b c|
}

TEST(TrieEncodeDict, LimitedFactorsProperly) {
    TrieEncodeDict d(3);
    // empty queue
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ( 0, d.try_char('a')); // a|
    // 1: a
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('a')); // a|a
    // 1: a
    ASSERT_EQ( 1, d.peek_codeword_no());
    ASSERT_EQ( 1, d.try_char('b')); // a|a b|
    // 1: a, 2: ab
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ( 0, d.try_char('c')); // a|a b|c|
    // 3: c, 1: a, 2: ab
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('a')); // a|a b|c|a
    // 3: c, 1: a, 2: ab
    ASSERT_EQ( 1, d.peek_codeword_no());
    ASSERT_EQ( 1, d.try_char('c')); // a|* *|c|a c|
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('c')); // a|* *|c|a c|c
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 3, d.peek_codeword_no());
    ASSERT_EQ( 3, d.try_char('a')); // a|* *|c|* *|c a|
    // 3: c, 2: ca, 1: a
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('a')); // a|* *|c|* *|c a|a
    // 3: c, 2: ca, 1: a
    ASSERT_EQ( 1, d.peek_codeword_no());
    ASSERT_EQ( 1, d.try_char('c')); // a|* *|c|* *|* *|a c|
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('a')); // a|* *|*|* *|* *|a c|a
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 1, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('c')); // a|* *|*|* *|* *|a c|a c
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 2, d.peek_codeword_no());
    ASSERT_EQ( 2, d.try_char('b')); // a|* *|*|* *|* *|a c|a c b|
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('a')); // a|* *|*|* *|* *|a c|a c b|a
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 1, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('c')); // a|* *|*|* *|* *|a c|a c b|a c
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 2, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('b')); // a|* *|*|* *|* *|a c|a c b|a c b
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 3, d.peek_codeword_no());
    ASSERT_EQ( 3, d.try_char('d')); // a|* *|*|* *|* *|a c|a c b|* * * *|
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 0, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('a')); // a|* *|*|* *|* *|a c|a c b|* * * *|a
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 1, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('c')); // a|* *|*|* *|* *|a c|a c b|* * * *|a c
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 2, d.peek_codeword_no());
    ASSERT_EQ(-1, d.try_char('b')); // a|* *|*|* *|* *|a c|a c b|* * * *|a c b
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 3, d.peek_codeword_no());
    ASSERT_EQ( 3, d.try_char('d')); // a|* *|*|* *|* *|a c|a c b|* * * *|* * * *
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 0, d.peek_codeword_no());
}
