#include "prefix.hpp"

#include "../src/trie_dictionary.cpp"

TEST(TrieDictionary, UnlimitedFactorsProperly) {
    TrieDictionary dict;               // a a b a b b a c a b c
    ASSERT_EQ( 0, dict.try_char('a')); // a|a b a b b a c a b c
    ASSERT_EQ(-1, dict.try_char('a')); // a|a.b a b b a c a b c
    ASSERT_EQ( 1, dict.try_char('b')); // a|a b|a b b a c a b c
    ASSERT_EQ(-1, dict.try_char('a')); // a|a b|a.b b a c a b c
    ASSERT_EQ(-1, dict.try_char('b')); // a|a b|a b.b a c a b c
    ASSERT_EQ( 2, dict.try_char('b')); // a|a b|a b b|a c a b c
    ASSERT_EQ(-1, dict.try_char('a')); // a|a b|a b b|a.c a b c
    ASSERT_EQ( 1, dict.try_char('c')); // a|a b|a b b|a c|a b c
    ASSERT_EQ(-1, dict.try_char('a')); // a|a b|a b b|a c|a.b c
    ASSERT_EQ(-1, dict.try_char('b')); // a|a b|a b b|a c|a b.c
    ASSERT_EQ( 2, dict.try_char('c')); // a|a b|a b b|a c|a b c|
}

TEST(TrieDictionary, LimitedFactorsProperly) {
    TrieDictionary dict(3);            // a a b c a c a b a c b c
    // empty queue
    ASSERT_EQ( 0, dict.peek_codeword_no());
    ASSERT_EQ( 0, dict.try_char('a')); // a|a b c a c a b a c b c
    // 1: a
    ASSERT_EQ( 0, dict.peek_codeword_no());
    ASSERT_EQ(-1, dict.try_char('a')); // a|a.b c a c a b a c b c
    // 1: a
    ASSERT_EQ( 1, dict.peek_codeword_no());
    ASSERT_EQ( 1, dict.try_char('b')); // a|a b|c a c a b a c b c
    // 1: a, 2: ab
    ASSERT_EQ( 0, dict.peek_codeword_no());
    ASSERT_EQ( 0, dict.try_char('c')); // a|a b|c|a c a b a c b c
    // 3: c, 1: a, 2: ab
    ASSERT_EQ( 0, dict.peek_codeword_no());
    ASSERT_EQ(-1, dict.try_char('a')); // a|a b|c|a.c a b a c b c
    // 3: c, 1: a, 2: ab
    ASSERT_EQ( 1, dict.peek_codeword_no());
    ASSERT_EQ( 1, dict.try_char('c')); // a|* *|c|a c|a b a c b c
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 0, dict.peek_codeword_no());
    ASSERT_EQ(-1, dict.try_char('a')); // a|* *|c|a c|a.b a c b c
    // 1: a, 2: ac, 3: c
    ASSERT_EQ( 1, dict.peek_codeword_no());
    ASSERT_EQ( 1, dict.try_char('b')); // a|* *|*|a c|a b|a c b c
    // 1: a, 3: ab, 2: ac
    ASSERT_EQ( 0, dict.peek_codeword_no());
    ASSERT_EQ(-1, dict.try_char('a')); // a|* *|*|a c|a b|a.c b c
    // 1: a, 3: ab, 2: ac
    ASSERT_EQ( 1, dict.peek_codeword_no());
    ASSERT_EQ(-1, dict.try_char('c')); // a|* *|*|a c|a b|a c.b c
    // 1: a, 3: ab, 2: ac
    ASSERT_EQ( 2, dict.peek_codeword_no());
    ASSERT_EQ( 2, dict.try_char('b')); // a|* *|*|a c|* *|a c b|c
    // 1: a, 2: ac, 3: acb
    ASSERT_EQ( 0, dict.peek_codeword_no());
    ASSERT_EQ( 0, dict.try_char('c')); // a|* *|*|a c|* *|* * *|c
    ASSERT_EQ( 0, dict.peek_codeword_no());
    // 3: c, 1: a, 2: ac
}
