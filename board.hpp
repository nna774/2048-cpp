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
#include <functional>
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
    using Grid = std::array<std::array<int,4>,4>;
    static Grid rotate(Grid const&, Dir);
    static Grid moveUp(Grid const&);
    static Grid moved(Grid const&, Dir);
    static bool movable(Grid const&, Dir);
    static std::pair<bool,Grid> movedAndBirth(Grid const& , Dir);
    static bool alive(Grid const&);
    static int log2(int);
    static int pow2(int);
    template<class T>
    using GridList_t = std::vector<T>;
    using GridList = GridList_t<Grid>;
    static GridList_t<std::pair<Grid, Dir>> nextPossibleWorld(Grid const&);
private:
    std::string const endpoint;
    int fd;
    std::string sessionID;
    std::random_device seed_gen;
    int toDead(std::pair<bool,Grid> const&);
    Grid grid;
    static void moveUpImp(std::array<int,4>&);
    static GridList nextPossibleWorldUp(Grid const&);
};

