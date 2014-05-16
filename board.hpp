#pragma once

#include <string>
#include <array>
#include <utility>
#include <algorithm>

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
    using Grid = uint64_t;
    static int get(Grid, int i, int j); // grid[i][j]
    static Grid set(Grid, int i, int j, int v);
    static Grid& set(Grid&, int i, int j, int v);
    static Grid rotate(Grid, Dir);
    static Grid moveUp(Grid);
    static Grid moved(Grid, Dir);
    static bool movable(Grid, Dir);
    static std::pair<bool,Grid> movedAndBirth(Grid, Dir);
    static bool alive(Grid);
    static int log2(int);
    static int pow2(int);
    template<class T>
    using GridList_t = std::vector<T>;
    using GridList = GridList_t<Grid>;
    static GridList_t<std::pair<Grid, Dir>> nextPossibleWorld(Grid);
private:
    std::string const endpoint;
    int fd;
    std::string sessionID;
    std::random_device seed_gen;
    int toDead(std::pair<bool,Grid>);
    Grid grid;
    static int moveUpImp(int);
    static GridList nextPossibleWorldUp(Grid);
};

