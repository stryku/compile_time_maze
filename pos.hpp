#pragma once

#include <cstdlib>

namespace cexpr
{
    struct pos
    {
        size_t x, y;

        static constexpr pos bad()
        {
            return pos{ static_cast<size_t>(-1), 0 };
        }

        constexpr bool is_bad() const
        {
            return x == static_cast<size_t>(-1);
        }

        constexpr bool operator==(const pos& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        constexpr bool operator!=(const pos& rhs) const
        {
            return !(*this == rhs);
        }
    };
}