#include "prefix.h"

#include "../src/smru_dict.cpp"

TEST (SmruDictTest, Encoding) {
    SmruEncodeDict d(3);
    ASSERT_EQ(Match(0, 0, 'a'), d.try_char('a'));
    // a|
    // 1: a
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|a
    // 1: a
    ASSERT_EQ(Match(1, 1, 'b'), d.try_char('b'));
    // a|a b|
    // 2: ab, 1: a
    ASSERT_EQ(Match(0, 0, 'c'), d.try_char('c'));
    // a|a b|c|
    // 2: ab, 1: a, 3: c
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|a b|c|a
    // 2: ab, 1: a, 3: c
    ASSERT_EQ(Match(1, 1, 'c'), d.try_char('c'));
    // a|* *|c|a c|
    // 3: c, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|* *|c|a c|a
    // 3: c, 2: ac, 1: a
    d.make_permanent(3);
    // a|* *|c|a c|a
    // 2: ac, 1: a, (3: c)
    ASSERT_EQ(Match(1, 1, 'b'), d.try_char('b'));
    // a|* *|c|* *|a b|
    // 2: ab, 1: a, (3: c)
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // a|* *|c|* *|a b|c
    // 2: ab, 1: a, (3: c)
    ASSERT_EQ(Match(3, 1, 'b'), d.try_char('b'));
    // a|* *|c|* *|* *|c b|
    // 1: a, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // a|* *|c|* *|a b|c b|c
    // 2: ab, 1: a, (3: c)
    ASSERT_FALSE(d.try_char('b').is_maximal());
    // a|* *|c|* *|* *|c b|c b
    // 1: a, 2: cb, (3: c)
    ASSERT_EQ(Match(2, 2, 'a'), d.try_char('a'));
    // *|* *|c|* *|* *|c b|c b a|
    // 1: cba, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // *|* *|c|* *|* *|c b|c b a|c
    // 1: cba, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('b').is_maximal());
    // *|* *|c|* *|* *|c b|c b a|c b
    // 1: cba, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // *|* *|c|* *|* *|c b|c b a|c b a
    // 1: cba, 2: cb, (3: c)
    ASSERT_EQ(Match(1, 3, 'd'), d.try_char('d'));
    // *|* *|c|* *|* *|c b|c b a|* * * *|
    // 1: cba, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // *|* *|c|* *|* *|c b|c b a|* * * *|c
    // 1: cba, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('b').is_maximal());
    // *|* *|c|* *|* *|c b|c b a|* * * *|c b
    // 1: cba, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // *|* *|c|* *|* *|c b|c b a|* * * *|c b a
    // 1: cba, 2: cb, (3: c)
    ASSERT_EQ(Match(1, 3, 'd'), d.try_char('d'));
    // *|* *|c|* *|* *|c b|c b a|* * * *|* * * *|
    // 1: cba, 2: cb, (3: c)
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // *|* *|c|* *|* *|c b|c b a|* * * *|* * * *|c
    // 1: cba, 2: cb, (3: c)
    Match match = d.fail_char();
    EXPECT_EQ(3, match.codeword_no);
    EXPECT_EQ(1, match.length);
    // *|* *|c|* *|* *|c b|c b a|* * * *|* * * *|c$
    // 1: cba, 2: cb, (3: c)
}

TEST (SmruDictTest, Decoding) {
    SmruDecodeDict d(3);

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

    d.make_permanent(1);   // 3: 4, 2: 2, (1: 1)

    d.add_extension(3, 5); // 3: 4, 2: 2, (1: 1) (rejected)

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 1), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(4, 2), d.codeword(3));

    d.add_extension(1, 6); // 2: 2, 3: 6, (1: 1)

    ASSERT_EQ(Codeword(0, 0), d.codeword(0));
    ASSERT_EQ(Codeword(1, 1), d.codeword(1));
    ASSERT_EQ(Codeword(2, 1), d.codeword(2));
    ASSERT_EQ(Codeword(6, 2), d.codeword(3));
}
