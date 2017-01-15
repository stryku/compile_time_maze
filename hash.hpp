#pragma once

#include "utils.hpp"

namespace cexpr
{
    template <size_t w, size_t h, size_t hash>
    constexpr maze_array<w, h> disassemble_hash()
    {
        maze_array<w, h> arr{};
        size_t mask = 1;

        for (size_t y = 0; y < h; ++y)
            for(size_t x = 0; x < w; ++x)
            {
                arr[y][x] = (hash & mask) * WALL;
                mask *= 2;
            }

        return arr;
    };
}