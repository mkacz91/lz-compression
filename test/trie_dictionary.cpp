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
