#include <limits.h>
#include "board.hpp"
#include "gtest/gtest.h"

TEST(BoardTest, log2){
    EXPECT_EQ(0, Board::log2(0));
    EXPECT_EQ(1, Board::log2(2));
    EXPECT_EQ(2, Board::log2(4));
    EXPECT_EQ(3, Board::log2(8));
    EXPECT_EQ(4, Board::log2(16));
    EXPECT_EQ(5, Board::log2(32));
    EXPECT_EQ(6, Board::log2(64));
    EXPECT_EQ(7, Board::log2(128));
    EXPECT_EQ(8, Board::log2(256));
    EXPECT_EQ(9, Board::log2(512));
    EXPECT_EQ(10, Board::log2(1024));
    EXPECT_EQ(11, Board::log2(2048));
    EXPECT_EQ(12, Board::log2(4096));
    EXPECT_EQ(13, Board::log2(8192));
    EXPECT_EQ(14, Board::log2(16384));
    EXPECT_EQ(15, Board::log2(32768));
    EXPECT_EQ(16, Board::log2(65536));
}
