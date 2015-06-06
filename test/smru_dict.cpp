#include "prefix.h"

#include "../src/smru_dict.cpp"

// TODO: There are no tests for DictBase::mark_as_permanent()

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
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // a|* *|c|a c|c
    // 3: c, 2: ac, 1: a
    ASSERT_EQ(Match(3, 1, 'a'), d.try_char('a'));
    // a|* *|c|* *|c a|
    // 1: a, 2: ca, 3: c
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|* *|c|* *|c a|a
    // 1: a, 2: ca, 3: c
    ASSERT_EQ(Match(1, 1, 'c'), d.try_char('c'));
    // a|* *|c|* *|* *|a c|
    // 3: c, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|* *|*|* *|* *|a c|a
    // 3: c, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // a|* *|*|* *|* *|a c|a c
    // 3: c, 2: ac, 1: a
    ASSERT_EQ(Match(2, 2, 'b'), d.try_char('b'));
    // a|* *|*|* *|* *|a c|a c b|
    // 3: acb, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|* *|*|* *|* *|a c|a c b|a
    // 3: acb, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // a|* *|*|* *|* *|a c|a c b|a c
    // 3: acb, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('b').is_maximal());
    // a|* *|*|* *|* *|a c|a c b|a c b
    // 3: acb, 2: ac, 1: a
    ASSERT_EQ(Match(3, 3, 'd'), d.try_char('d'));
    // a|* *|*|* *|* *|a c|a c b|* * * *|
    // 3: acb, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|* *|*|* *|* *|a c|a c b|* * * *|a
    // 3: acb, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('c').is_maximal());
    // a|* *|*|* *|* *|a c|a c b|* * * *|a c
    // 3: acb, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('b').is_maximal());
    // a|* *|*|* *|* *|a c|a c b|* * * *|a c b
    // 3: acb, 2: ac, 1: a
    ASSERT_EQ(Match(3, 3, 'd'), d.try_char('d'));
    // a|* *|*|* *|* *|a c|a c b|* * * *|* * * *|
    // 3: acb, 2: ac, 1: a
    ASSERT_FALSE(d.try_char('a').is_maximal());
    // a|* *|*|* *|* *|a c|a c b|* * * *|* * * *|a
    // 3: acb, 2: ac, 1: a
    Match match = d.fail_char();
    ASSERT_EQ(1, match.codeword_no);
    ASSERT_EQ(1, match.length);
    // a|* *|*|* *|* *|a c|a c b|* * * *|* * * *|a$
    // 3: acb, 2: ac, 1: a
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

    d.add_extension(2, 4); // 3: 4, 2: 2, 1: 1

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
