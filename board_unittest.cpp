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

TEST(BoardTest, pow2){
    EXPECT_EQ(0, Board::pow2(0));
    EXPECT_EQ(2, Board::pow2(1));
    EXPECT_EQ(4, Board::pow2(2));
    EXPECT_EQ(8, Board::pow2(3));
    EXPECT_EQ(16, Board::pow2(4));
    EXPECT_EQ(32, Board::pow2(5));
    EXPECT_EQ(64, Board::pow2(6));
    EXPECT_EQ(128, Board::pow2(7));
    EXPECT_EQ(256, Board::pow2(8));
    EXPECT_EQ(512, Board::pow2(9));
    EXPECT_EQ(1024, Board::pow2(10));
    EXPECT_EQ(2048, Board::pow2(11));
    EXPECT_EQ(4096, Board::pow2(12));
    EXPECT_EQ(8192, Board::pow2(13));
    EXPECT_EQ(16384, Board::pow2(14));
    EXPECT_EQ(32768, Board::pow2(15));
    EXPECT_EQ(65536, Board::pow2(16));
}
