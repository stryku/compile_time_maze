#include <iostream>
#include <cstdlib>
#include <array>

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
                arr[y][x] = (hash & mask) ? WALL : 0;
                mask *= 2;
            }

        return arr;
    };
}

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

namespace cexpr
{
    namespace details
    {
        template <size_t w, size_t h>
        using path_t = max_array<char, w*h>;

        template <size_t w, size_t h>
        constexpr max_array<pos, w*h> find_path_impl(pos current,
                                                     pos to,
                                                     maze_array<w,h>& current_maze,
                                                     max_array<pos, w*h> &current_path,
                                                     size_t current_length = 1)
        {
            current_maze[current.y][current.x] = current_length;

            if(current == to)
                return current_path;

            current_path.push(current);

            const auto neighb = maze::get_neighbours(current, current_maze);

            for(size_t i = 0; i < neighb.count; ++i)
            {
                const auto neighbour_pos = neighb.values[i];
                current_maze[neighbour_pos.y][neighbour_pos.x] = current_length+1;
                if(neighbour_pos == to)
                {
                    current_path.push(neighbour_pos);
                    return current_path;
                }
            }

            for(size_t i = 0; i < neighb.count; ++i)
                return find_path_impl(neighb.values[i], to, current_maze, current_path, current_length + 1);

            current_path.pop();

            return current_path;
        };

        template <size_t w, size_t h>
        constexpr auto create_bad_path_message()
        {
            max_array<char, w*h> path{};

            path.push('n');

            return path;
        };

        constexpr char direction(pos from, pos to)
        {
            if(from.x == to.x - 1) return 'r';
            if(from.x == to.x + 1) return 'l';
            if(from.y == to.y - 1) return 'd';
            if(from.y == to.y + 1) return 'u';

            return '\0';
        }

        template <size_t w, size_t h>
        constexpr auto translate_path(max_array<pos, w*h> &path)
        {
            max_array<char, w*h> translated_path{};

            for(size_t i = 0; i < path.count-1; ++i)
            {
                const char dir = direction(path.values[i], path.values[i+1]);

                if(dir == '\0')
                    return create_bad_path_message<w,h>();

                translated_path.push(dir);
            }

            return translated_path;
        };
    }



    template <size_t w, size_t h, size_t hash>
    constexpr auto find_path(pos from, pos to)
    {
        maze_array<w, h> maze{};
        maze = disassemble_hash<w, h, hash>();

        if(maze[from.y][from.x] == WALL)
            return details::create_bad_path_message<w, h>();

        max_array<pos, w*h> path{};

        path = details::find_path_impl(from, to, maze, path);

        if(path.count > 0 && path.back() != to)
            return details::create_bad_path_message<w, h>();

        return details::translate_path<w, h>(path);
    };
}


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

#ifndef WIDTH
#define WIDTH 3
#endif

#ifndef HEIGHT
#define HEIGHT 3
#endif

#ifndef FROM_X
#define FROM_X 0
#endif

#ifndef FROM_Y
#define FROM_Y 0
#endif

#ifndef TO_X
#define TO_X 2
#endif

#ifndef TO_Y
#define TO_Y 2
#endif

constexpr auto paths = cexpr::generate_paths<WIDTH,HEIGHT>(cexpr::pos{FROM_X, FROM_Y},
                                                           cexpr::pos{TO_X, TO_Y});

#include <bitset>

int main()
{
    size_t hash{ 0 };
    size_t mask{ 1 };

    for(size_t i = 0; i < WIDTH*HEIGHT; ++i, mask *= 2)
    {
        char c;
        std::cin>>c;

        size_t or_value = (c == '1') ? mask : 0;
        hash |= or_value;
    }
    
    const auto& path = paths[hash];

    for(int i = path.count - 1; i >= 0; --i)
        std::cout<<path.values[i];

    return 0;
}

