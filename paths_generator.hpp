#pragma once

#include "path_finder.hpp"

#include <utility>
#include <iostream>

namespace cexpr
{
    namespace details
    {
        template <size_t w, size_t h, size_t ...hashes>
        constexpr auto generate_paths_impl(pos from, pos to, std::index_sequence<hashes...>)
        {
            return std::array<path_t<w,h>, sizeof...(hashes)>{
                    find_path<w, h, hashes>(from, to)... ,
            };
        }
    }

    template<size_t w, size_t h>
    constexpr auto generate_paths(pos from, pos to)
    {
        return details::generate_paths_impl<w, h>(from,
                                                  to,
                                                  std::make_index_sequence<max_hash_value_calculator<w,h>::value>{});
    };
}
