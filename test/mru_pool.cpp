#include "prefix.hpp"

#include "../src/mru_pool.hpp"

TEST(MruPool, FiniteWorksCorrectly) {
    MruPool pool(4);
    ASSERT_EQ(0, pool.get()); // 0
    pool.use(0);              // 0
    ASSERT_EQ(1, pool.get()); // 1 0
    ASSERT_EQ(2, pool.get()); // 2 1 0
    pool.use(1);              // 1 2 0
    ASSERT_EQ(3, pool.get()); // 3 1 2 0
    ASSERT_EQ(0, pool.get()); // 0 3 1 2
    pool.use(2);              // 2 0 3 1
    ASSERT_EQ(1, pool.get()); // 1 2 0 3
    ASSERT_EQ(3, pool.get()); // 3 1 2 0
}

TEST(MruPool, InfiniteWorksCorrectly) {
    MruPool pool;
    for (int i = 0; i < 100; ++i) {
        ASSERT_EQ(i, pool.get());
        if (i % 5 == 0)
            pool.use(i / 5);
    }
}
