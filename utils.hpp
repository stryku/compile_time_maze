#pragma once

#include <cstdlib>
#include <array>

namespace cexpr
{
    constexpr size_t WALL = static_cast<size_t>(-1);

    template <size_t w, size_t h>
    using maze_array = std::array<std::array<size_t, w>, h>;

    template <typename T, size_t max>
    struct max_array
    {
        std::array<T, max> values;
        size_t count{ 0 };

        constexpr void push(T v)
        {
            values[count++] = v;
        }
    };
}