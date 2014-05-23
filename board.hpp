#pragma once

#include <string>
#include <array>
#include <utility>
#include <algorithm>
#include <cstdint>

enum class Dir{
    Up,
    Right,
    Down,
    Left,
};

int constexpr dirToInt(Dir dir){
    return dir == Dir::Up ? 0 :
        dir == Dir::Right ? 1 :
        dir == Dir::Down ? 2 : 3;
}

static std::array<Dir,4> const constexpr allDirs = {{ Dir::Up, Dir::Right, Dir::Down, Dir::Left }};

Dir constexpr mirror(Dir dir){
    /* if (dir == Dir::Up || dir == Dir::Down) return dir;
     * if (dir == Dir::Left) return Dir::Right;
     * if (dir == Dir::Right) return Dir::Left; */
    return (dir == Dir::Up || dir == Dir::Down) ? dir : (dir == Dir::Left) ? Dir::Right : Dir::Left;
}
namespace {
    std::array<uint16_t, 1 << 16>  makeTable();
}

class Board{
public:
    Board();
    Board(std::string const protocol, std::string const endpoint, std::string const port);
    bool move(Dir dir); // return value is moved
    // Dir decideDir();
    template<class T> Dir decideDir();
    using Grid = uint64_t;
    void show() const;
    static void show(Grid);
    static int get(Board::Grid grid, int i, int j){
        int pos = i * 16 + j * 4;
        return (grid >> pos) & 0b1111;
    }
    static Grid set(Board::Grid const grid, int i, int j, int v){
        int pos = i * 16 + j * 4;
        return (grid & ~(UINT64_C(0b1111) << pos)) | ((v & (UINT64_C(0b1111))) << pos);
    }
    static Grid rotate(Grid, Dir);
    static Grid transpose(Grid);
    static Grid gridMirror(Grid);
    static Grid moveLeft(Board::Grid grid){
        static std::array<uint16_t, 1 << 16> const table = makeTable();
        return
            ((uint64_t)table[(grid & UINT64_C(0xFFFF000000000000)) >> 48] << 48) |
            ((uint64_t)table[(grid & UINT64_C(0x0000FFFF00000000)) >> 32] << 32) |
            ((uint64_t)table[(grid & UINT64_C(0x00000000FFFF0000)) >> 16] << 16) |
            ((uint64_t)table[(grid & UINT64_C(0x000000000000FFFF)) >> 00] << 00) ;
    }
    static Grid moved(Board::Grid grid, Dir dir){
        if(dir == Dir::Up) return transpose(moveLeft(transpose(grid)));
        if(dir == Dir::Down) return transpose(gridMirror(moveLeft(gridMirror(transpose(grid)))));
        if(dir == Dir::Right) return gridMirror(moveLeft(gridMirror(grid)));
        return moveLeft(grid);
    }
    static bool movable(Board::Grid grid, Dir dir){
        return moved(grid, dir) != grid;
    }
    static std::pair<bool,Grid> movedAndBirth(Grid, Dir);
    static bool alive(Board::Grid grid) {
        for(int i(0); i < 4; ++i)
            if(movable(grid, allDirs[i]))
                return true;
        return false;
    }
    static int log2(int i){
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
            return 5;
        case 64:
            return 6;
        case 128:
            return 7;
        case 256:
            return 8;
        case 512:
            return 9;
        case 1024:
            return 10;
        case 2048:
            return 11;
        case 4096:
            return 12;
        case 8192:
            return 13;
        case 16384:
            return 14;
        default:
            return 15;
        }
    }
    static int pow2(int i) {
        static int const constexpr table[] = {0,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
        return table[i];
    }
    static int countZeroGrid(Grid grid){
        int cnt(0);
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
                if(get(grid, i, j) == 0) ++cnt;
        return cnt;
    }
private:
    std::string const endpoint;
    int fd;
    std::string sessionID;
    std::random_device seed_gen;
    int toDead(std::pair<bool,Grid>);
public:
    Grid grid;
    static int moveUpImp(int);
};

namespace {
    std::array<uint16_t, 1 << 16>  makeTable(){
        std::array<uint16_t, 1 << 16> table;
        auto const constexpr k = 0;
        for(int orig(0); orig < 1 << 16;++orig){
            auto grid = orig;
            bool joined = false;
            bool hit = false;
            for(int i(0); i < 4;++i){
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
                }
            }
            table[orig] = grid;
        }
        return table;
    }
};

