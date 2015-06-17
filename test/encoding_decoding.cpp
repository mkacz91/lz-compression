#include "prefix.h"

#include "../src/buffer.h"
#include "../src/lz78.h"
#include "../src/lzw.h"
#include "../src/mra_dict.h"
#include "../src/smru_dict.h"
#include "../src/wmru_dict.h"

template <typename Lz>
class EncodeDecodeTest : public testing::Test {
protected:
    Buffer single_char;
    Buffer lorem_ipsum;
    Buffer alphabet;

    EncodeDecodeTest ();

    void init_single_char ();

    void init_lorem_ipsum ();

    void init_alphabet ();
};

template <typename Lz>
EncodeDecodeTest<Lz>::EncodeDecodeTest () {
    init_single_char();
    init_lorem_ipsum();
    init_alphabet();
}

template <typename Lz>
void EncodeDecodeTest<Lz>::init_single_char () {
    BufferCharWriter writer(single_char);
    writer.put('x');
}

template <typename Lz>
void EncodeDecodeTest<Lz>::init_lorem_ipsum () {
    BufferCharWriter writer(lorem_ipsum);
    writer.put(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec "
        "interdum cursus venenatis. Proin eget fringilla nulla, ut sagittis "
        "sem. Pellentesque habitant morbi tristique senectus et netus et "
        "malesuada fames ac turpis egestas. Nunc ultrices erat sit amet leo "
        "accumsan congue. Duis faucibus justo felis, vel pulvinar enim tempus "
        "condimentum. Sed aliquam placerat nisi, vitae posuere eros finibus "
        "sed. Aenean vestibulum et metus eu congue. Pellentesque nec velit ut "
        "purus suscipit dapibus. Interdum et malesuada fames ac ante ipsum "
        "primis in faucibus. Nam eu rhoncus quam. Suspendisse consectetur "
        "neque turpis, vitae pellentesque quam vulputate sed."
    );
}

template <typename Lz>
void EncodeDecodeTest<Lz>::init_alphabet () {
    BufferCharWriter writer(alphabet);
    for (int i = 0; i < 15; ++i) {
        for (int a = 0; a < CHAR_CNT; ++a)
            writer.put(a);
    }
}

typedef testing::Types<
    Lz78<Smru>,
    Lzw<Smru>,
    Lz78<Mra>,
    Lzw<Mra>,
    Lz78<Wmru>,
    Lzw<Wmru>,
    Lz78<Smru2>,
    Lzw<Smru2>
> algos;
TYPED_TEST_CASE(EncodeDecodeTest, algos);

TYPED_TEST (EncodeDecodeTest, SingleChar) {
    TypeParam lz(10);
    ASSERT_EQ(this->single_char, lz.decode(lz.encode(this->single_char)));
}

TYPED_TEST (EncodeDecodeTest, LoremIpsum) {
    TypeParam lz(10);
    ASSERT_EQ(this->lorem_ipsum, lz.decode(lz.encode(this->lorem_ipsum)));
}

TYPED_TEST (EncodeDecodeTest, Alphabet) {
    TypeParam lz(40);
    ASSERT_EQ(this->alphabet, lz.decode(lz.encode(this->alphabet)));
}
