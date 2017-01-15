#pragma once

#include "utils.hpp"
#include "hash.hpp"
#include "pos.hpp"
#include "maze.hpp"


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
