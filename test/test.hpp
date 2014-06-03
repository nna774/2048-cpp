#include <random>
#include <vector>

namespace test{
    std::mt19937 mtInit(){
        static std::random_device rnd;
        std::vector<std::uint_least32_t> v(10);
        for(auto& e: v) e = rnd();
        std::seed_seq seq(begin(v), end(v));
        std::mt19937 mt(seq);
        return mt;
    }
};

