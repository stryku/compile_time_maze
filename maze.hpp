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
            neighbours neighb{};

            const auto positions = {
                    pos{ p.x, p.y - 1 },
                    pos{ p.x, p.y + 1 },
                    pos{ p.x - 1, p.y },
                    pos{ p.x + 1, p.y }
            };

            for(const auto& p : positions)
                if(is_in(p, maze) && maze[p.y][p.x] == desired_value)
                    neighb.push(p);

            return neighb;
        };
    }
}
