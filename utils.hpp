#pragma once

#include <cstdlib>
#include <array>

namespace cexpr
{
    constexpr size_t WALL = static_cast<size_t>(-1);

    template <size_t w, size_t h>
    using maze_array = std::array<std::array<size_t, w>, h>;

    template <size_t w, size_t h>
    struct max_hash_value_calculator
    {
        static constexpr size_t value = 1 << (w * h + 1);
    };

    template <typename T, size_t max>
    struct max_array
    {
        std::array<T, max> values;
        size_t count{ 0 };

        constexpr void push(T v)
        {
            values[count++] = v;
        }

        constexpr void pop()
        {
            --count;
        }

        constexpr T& back()
        {
            return values[count-1];
        }
    };
}