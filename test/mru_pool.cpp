#include "prefix.hpp"

#include "../src/mru_pool.hpp"

TEST(MruPool, FiniteWorksCorrectly) {
    MruPool pool(4);
    ASSERT_EQ(0, pool.get()); // 0
    pool.use(0);              // 0
    ASSERT_EQ(1, pool.get()); // 0 1
    ASSERT_EQ(2, pool.get()); // 0 1 2
    pool.use(1);              // 1 0 2
    ASSERT_EQ(3, pool.get()); // 1 0 2 3
    ASSERT_EQ(3, pool.get()); // 1 0 2 3
    pool.use_before(0, 3);    // 1 0 3 2
    ASSERT_EQ(2, pool.get()); // 1 0 3 2
    pool.use(2);              // 2 1 0 3
    ASSERT_EQ(3, pool.get()); // 2 1 0 3
    pool.use(3);              // 3 2 1 0
    pool.use(0);              // 0 3 2 1
    ASSERT_EQ(1, pool.get()); // 0 3 2 1
}

TEST(MruPool, InfiniteWorksCorrectly) {
    MruPool pool;
    for (int i = 0; i < 100; ++i)
        ASSERT_EQ(i, pool.get());
}
