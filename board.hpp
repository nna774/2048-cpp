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
    static Grid set(Board::Grid const grid, int i, int j, uint64_t v){
        int pos = i * 16 + j * 4;
        return (grid & ~(UINT64_C(0b1111) << pos)) | (v << pos); // v が4bit 超えてると死ぬ
        // return (grid & ~(UINT64_C(0b1111) << pos)) | (v & (UINT64_C(0b1111)) << pos);
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
        static std::array<uint16_t, 1 << 16> const table = Board::makeTable().first;
        return
            (static_cast<uint64_t>(table[(grid & UINT64_C(0xFFFF000000000000)) >> 48]) << 48) |
            (static_cast<uint64_t>(table[(grid & UINT64_C(0x0000FFFF00000000)) >> 32]) << 32) |
            (static_cast<uint64_t>(table[(grid & UINT64_C(0x00000000FFFF0000)) >> 16]) << 16) |
            (static_cast<uint64_t>(table[(grid & UINT64_C(0x000000000000FFFF)) >> 00]) << 00) ;
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
    static std::mt19937 mtInit(){
        std::vector<std::uint_least32_t> v(10);
        static std::random_device rnd;
        for(auto& e: v) e = rnd();
        std::seed_seq seq(begin(v), end(v));
        std::mt19937 mt(seq);
        return mt;
    }
    static std::pair<bool,Grid> moveAndBirth(Board::Grid grid, Dir dir){
        static std::mt19937 mt = mtInit();

        auto moved = Board::moved(grid, dir);
        int zeros = Board::countZeroGrid(grid);
        bool flg(false);
        if(zeros > 0){
            flg = true;
            int point = mt() % zeros;
            int birth(0);
            if(mt() % 10) birth = 1;
            else birth = 2;
            for(int i(0); i < 4; ++i)
                for(int j(0); j < 4; ++j)
                    if(Board::get(moved, i, j) == 0)
                        if(point-- == 0) moved = Board::set(moved, i, j, birth);
        }
        return {flg, moved};
    }
    static bool alive(Board::Grid grid) {
        for(int i(0); i < 4; ++i)
            if(movable(grid, allDirs[i]))
                return true;
        return false;
    }
    static int popCount(int bits){
        bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
        bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
        bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
        bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
        return (bits & 0x0000ffff) + (bits >>16 & 0x0000ffff);
    }
    static int log2(int i){
        if(i == 0) return 0;
        return popCount((i & (-i)) - 1);
    }
    static int pow2(int i) {
        if(i == 0) return 0;
        return 1 << i;
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
    static std::pair<std::array<uint16_t, 1 << 16>, std::array<int, 1 << 16>> makeTable();
    static int getScore(Grid grid, Dir dir){
        static std::array<int, 1 << 16> const table = makeTable().second;
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
            table[(grid & UINT64_C(0xFFFF000000000000)) >> 48] +
            table[(grid & UINT64_C(0x0000FFFF00000000)) >> 32] +
            table[(grid & UINT64_C(0x00000000FFFF0000)) >> 16] +
            table[(grid & UINT64_C(0x000000000000FFFF)) >> 00] ;
    }
private:
    std::string const endpoint;
    int fd;
    std::string sessionID;
    std::random_device seed_gen;
    static std::array<bool, 1 << 16> makeMovableTable();
    Grid grid;
};

