#include <iostream>
#include "paths_generator.hpp"
#include "defines.hpp"

constexpr auto paths = cexpr::generate_paths<WIDTH,HEIGHT>(cexpr::pos{FROM_X, FROM_Y},
                                                           cexpr::pos{TO_X, TO_Y});

int main()
{
    size_t hash{ 0 };
    size_t mask{ 1 };

    for(size_t i = 0; i < WIDTH*HEIGHT; ++i, mask *= 2)
    {
        char c;
        std::cin>>c;

        size_t or_value = (c == 1) ? mask : 0;
        hash |= or_value;
    }

    const auto& path = paths[hash];

    for(int i = path.count - 1; i >= 0; --i)
        std::cout<<path.values[i];

    return 0;
}