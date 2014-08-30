#include <iostream>
#include "board.hpp"

#include "koyone.hpp"
#include "koyoneNext.hpp"
#include "kihime.hpp"
#include "kihimeNext.hpp"
#include "nona7.hpp"
#include "uct.hpp"

static std::string const protocol = "http";
static std::string const endpoint = "ring"; // #### TODO: 適当にargv 読むようにする ####
static std::string const port = "2048";

int main(int, char**){
    Board b("http", "2048.semantics3.com", "8080");
    // Board b(protocol, endpoint, port);
    b.show();

    try{
        while(1){
            // auto tmp = b.grid;
            auto dir = b.decideDir<KoyoneNext>();
            b.move(dir);
            b.show();
        }
    }
    catch (std::string v){
        std::cerr << v << std::endl;
        b.show();
    }
}

