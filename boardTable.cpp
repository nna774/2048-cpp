#include "board.hpp"

std::array<uint16_t, 1 << 16>  makeTable(){
    std::array<uint16_t, 1 << 16> table;
    auto const constexpr k = 0;
    for(int orig(0); orig < 1 << 16;++orig){
        auto grid = orig;
        bool joined = false;
        bool hit = false;
        for(int i(0); i < 4;++i){
            // joined = false;
            if(Board::get(grid, k, i) == 0) continue;
            hit = false;
            for(int j(i-1); j >= 0;--j){
                if(Board::get(grid, k, j) == 0) continue;
                if(Board::get(grid, k, j) == Board::get(grid, k, i) && !joined){
                    grid = Board::set(grid, k, j, Board::get(grid, k, j) + 1);
                    grid = Board::set(grid, k, i, 0);
                    joined = true;
                }else{
                    if(j + 1 != i){
                        grid = Board::set(grid, k, j+1, Board::get(grid, k, i));
                        grid = Board::set(grid, k, i, 0);
                    }
                    joined = false;
                }
                hit = true;
                break;
            }
            if(i != 0 && ! hit){
                grid = Board::set(grid, k, 0, Board::get(grid, k, i));
                grid = Board::set(grid, k, i, 0);
                // joined = false;
            }
        }
        // printf("%u\n", grid);
        table[orig] = grid;
    }
    return table;
}
Board::Grid Board::moveLeft(Board::Grid grid){
    static std::array<uint16_t, 1 << 16> const table = makeTable();
    return
        ((uint64_t)table[(grid & UINT64_C(0xFFFF000000000000)) >> 48] << 48) |
        ((uint64_t)table[(grid & UINT64_C(0x0000FFFF00000000)) >> 32] << 32) |
        ((uint64_t)table[(grid & UINT64_C(0x00000000FFFF0000)) >> 16] << 16) |
        ((uint64_t)table[(grid & UINT64_C(0x000000000000FFFF)) >> 00] << 00) ;
}
















