#include <limits.h>
#include "../board.hpp"
#include "../koyone.hpp"
#include "gtest/gtest.h"
#include <random>

#define ITERATION 10000000

TEST(KoyoneTest, staticEvalRAND){
    static std::random_device rnd;
    std::vector<std::uint_least32_t> v(10);
    for(auto& e: v) e = rnd();
    std::seed_seq seq(begin(v), end(v));
    std::mt19937 mt(seq);

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
        int sum(0);
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j){
                auto gotten = Board::get(grid, i, j);
                sum += gotten
                    ? Board::pow2(gotten) * gotten
                    : Koyone::SPACE_WEIGHT;
            }

        EXPECT_EQ(sum, Koyone::staticEval(grid));
    }
}
