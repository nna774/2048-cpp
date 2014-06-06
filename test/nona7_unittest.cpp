#include <limits.h>
#include "../board.hpp"
#include "../nona7.hpp"
#include "gtest/gtest.h"
#include "test.hpp"

#define ITERATION 1000000

namespace {
    int sqrt(int i){
	switch(i){
	case 0:
	    return 0;
	case 2:
	    return 1;
	case 4:
	    return 2;
	case 8:
	    return 3;
	case 16:
	    return 4;
	case 32:
	    return 6;
	case 64:
	    return 8;
	case 128:
	    return 11;
	case 256:
	    return 17;
	case 512:
	    return 23;
	case 1024:
	    return 32;
	case 2048:
	    return 45;
	case 4096:
	    return 64;
	case 8192:
	    return 91;
	case 16384:
	    return 128;
	default: 
	    return 181;
	};
    }
    int xsqrtx_pow2(int i){
	switch(i){
	case 0:
	    return 0;
	case 1:
	    return 3;
	case 2:
	    return 8;
	case 3:
	    return 23;
	case 4:
	    return 64;
	case 5:
	    return 181;
	case 6:
	    return 512;
	case 7:
	    return 1448;
	case 8:
	    return 4096;
	case 9:
	    return 11585;
	case 10:
	    return 32768;
	case 11:
	    return 92682;
	case 12:
	    return 262144;
	case 13:
	    return 741455;
	case 14:
	    return 2097152;
	case 15:
	default:
	    return 5931642;
	};
    }
}

TEST(Nona7Test, staticEvalRAND){
    std::mt19937 mt = test::mtInit();

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
	static int const valss[4][4][4] = {
	    {
		{80, 40, 40, 40},
		{30, 10, 10, 10},
		{03, 01, 01, 03},
		{01, 01, 01, 01}
	    },
	    {
		{40, 40, 40, 80},
		{10, 10, 10, 30},
		{03, 01, 01, 03},
		{01, 01, 01, 01}
	    },
	    {
		{80, 30, 03, 01},
		{40, 10, 01, 01},
		{40, 10, 01, 01},
		{40, 10, 03, 01}
	    },
	    {
		{01, 03, 30, 80},
		{01, 01, 10, 40}, 
		{01, 01, 10, 40},
		{01, 03, 10, 40}
	    }
	};
	std::array<int, 4> sums;
	sums.fill(0);
	for(int i(0); i < 4; ++i)
	    for(int j(0); j < 4; ++j)
		for(int k(0); k < 4; ++k)
		    sums[k] += xsqrtx_pow2(Board::get(grid, i, j)) * valss[k][i][j];

        EXPECT_EQ(*(std::max_element(std::begin(sums), std::end(sums))), Nona7::staticEval(grid));
    }
}
