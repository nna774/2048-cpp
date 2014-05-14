#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <cstdio>
#include <array>
#include <iomanip>
#include <random>
#include <utility>
#include <set>
#include <algorithm>
#include "picojson/picojson.h"

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
    void show() const;
    Dir decideDir();
private:
    using Grid = std::array<std::array<int,4>,4>;
    std::string const endpoint;
    int fd;
    std::string sessionID;
    std::random_device seed_gen;
    std::mt19937 mt;
    std::uniform_int_distribution<int> rand4;
    std::uniform_int_distribution<int> rand10;
    int toDead(std::pair<bool,Grid> const&);
    Grid grid;
    Grid rotate(Grid const&, Dir) const;
    void moveUpImp(std::array<int,4>&) const;
    Grid moveUp(Grid const&) const;
    Grid moved(Grid const&, Dir) const;
    bool movable(Grid const&, Dir) const;
    std::pair<bool,Grid> movedAndBirth(Grid const& , Dir);
    bool alive(Grid const&) const;
    static int staticEval(Grid const&);
    static int log2(int);
    struct Comp{
        bool operator()(std::pair<Grid, Dir> const& a, std::pair<Grid, Dir> const& b){
            return staticEval(a.first) < staticEval(b.first);
        }
        bool operator()(Grid const& a, Grid const& b){
            return staticEval(a) < staticEval(b);
        }
    };
    template<class T>
    using GridList_t = std::vector<T>;
    using GridList = GridList_t<Grid>;
    GridList_t<std::pair<Grid, Dir>> nextPossibleWorld(Grid const&) const;
    GridList nextPossibleWorldUp(Grid const&) const;
    static bool nurseryTime(Grid const&);
};









