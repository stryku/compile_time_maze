#include <iostream>
#include "maze.hpp"
#include "path_finder.hpp"

using namespace cexpr;


constexpr auto path = find_path<3, 3, 0>(pos{0,0}, pos{2,2});

int main()
{

    for(size_t i = 0; i < path.count; ++i)
        std::cout<<path.values[i];

    return 0;
}