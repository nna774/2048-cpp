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
    static Grid transpose(Board::Grid grid){
        static uint64_t const
            cdgh = UINT64_C(0x00FF00FF00000000),
            ijmn = UINT64_C(0x00000000FF00FF00),
            bdjl = UINT64_C(0x0F0F00000F0F0000),
            egmo = UINT64_C(0x0000F0F00000F0F0);
        grid = (grid & ~cdgh & ~ijmn) | ((grid & cdgh) >> 24) | ((grid & ijmn) << 24);
        return (grid & ~bdjl & ~egmo) | ((grid & bdjl) >> 12) | ((grid & egmo) << 12);
    }
    static Grid gridMirror(Board::Grid grid){
        static uint64_t const
            aeim = UINT64_C(0xF000F000F000F000),
            bfjn = UINT64_C(0x0F000F000F000F00),
            cgko = UINT64_C(0x00F000F000F000F0),
            dhlp = UINT64_C(0x000F000F000F000F);
        return
            ((grid & aeim) >> 12)|
            ((grid & bfjn) >> 4) |
            ((grid & cgko) << 4) |
            ((grid & dhlp) << 12);
    }
    static Grid moveLeft(Board::Grid grid){
        static std::array<uint16_t, 1 << 16> const table = Board::makeTable();
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
        static std::array<bool, 1 << 16> const movableTable = makeMovableTable();
        switch(dir){
        case Dir::Left:
            break;
        case Dir::Right:
            grid = Board::gridMirror(grid);
            break;
        case Dir::Up:
            grid = Board::transpose(grid);
            break;
        case Dir::Down:
        default:
            grid = Board::gridMirror(Board::transpose(grid));
            break;
        }
        return
            movableTable[(grid & UINT64_C(0xFFFF000000000000)) >> 48] |
            movableTable[(grid & UINT64_C(0x0000FFFF00000000)) >> 32] |
            movableTable[(grid & UINT64_C(0x00000000FFFF0000)) >> 16] |
            movableTable[(grid & UINT64_C(0x000000000000FFFF)) >> 00] ;
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
        grid = ((grid & UINT64_C(0xCCCCCCCCCCCCCCCC)) >> 2) | (grid & UINT64_C(0x3333333333333333));
        grid = ((grid & UINT64_C(0x2222222222222222)) >> 1) | (grid & UINT64_C(0x1111111111111111));

        grid = ((grid & UINT64_C(0x1010101010101010)) >> 4) + (grid & UINT64_C(0x0101010101010101));
        grid = grid + (grid >> 32);
        grid = grid + (grid >> 16);
        grid = grid + (grid >> 8);
        return 16 - (grid & 0xFF);
    }
    static std::array<uint16_t, 1 << 16> makeTable();
private:
    std::string const endpoint;
    int fd;
    std::string sessionID;
    std::random_device seed_gen;
    int toDead(std::pair<bool,Grid>);
    static std::array<bool, 1 << 16> makeMovableTable();
    Grid grid;
    static int moveUpImp(int);
};

