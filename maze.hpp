#pragma once

#include "pos.hpp"
#include "utils.hpp"

namespace cexpr
{
    namespace maze
    {
        template <size_t w, size_t h>
        constexpr bool is_in(pos p, maze_array<w,h>)
        {
            return p.x < w && p.y < h;
        };

        using neighbours = max_array<pos, 4>;

        template <size_t w, size_t h>
        constexpr auto get_neighbours(pos p, maze_array<w,h> &maze, size_t desired_value = 0)
        {
            neighbours n{};


            if(is_in(pos{ p.x, p.y - 1 }, maze) &&
               maze[p.y - 1][p.x] == desired_value)
            {
                n.values[n.count++] = pos{p.x, p.y - 1};
            }

            if(is_in(pos{ p.x, p.y + 1 }, maze) &&
               maze[p.y + 1][p.x] == desired_value)
            {
                n.values[n.count++] = pos{p.x, p.y + 1};
            }

            if(is_in(pos{ p.x - 1, p.y }, maze) &&
               maze[p.y][p.x - 1] == desired_value)
            {
                n.values[n.count++] = pos{p.x - 1, p.y};
            }

            if(is_in(pos{ p.x + 1, p.y }, maze) &&
               maze[p.y][p.x + 1] == desired_value)
            {
                n.values[n.count++] = pos{p.x + 1, p.y};
            }

            return n;
        };
    }

}
