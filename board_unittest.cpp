#include <limits.h>
#include "board.hpp"
#include "gtest/gtest.h"

TEST(BoardTest, log2){
    EXPECT_EQ(0, Board::log2(0));
}
