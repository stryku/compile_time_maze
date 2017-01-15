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
        constexpr void find_path_impl(pos current,
                                      pos to,
                                      maze_array<w,h>& current_maze,
                                      size_t current_length = 1)
        {
            current_maze[current.y][current.x] = current_length;

            if(current == to)
                return;

            const auto neighb = maze::get_neighbours(current, current_maze);

            for(size_t i = 0; i < neighb.count; ++i)
            {
                const auto neighbour_pos = neighb.values[i];
                current_maze[neighbour_pos.y][neighbour_pos.x] = current_length+1;
                if(neighbour_pos == to)
                    return;
            }

            for(size_t i = 0; i < neighb.count; ++i)
                find_path_impl(neighb.values[i], to, current_maze, current_length + 1);
        };

        template <typename maze_t>
        constexpr pos find_next_path_tile(pos current, maze_t maze)
        {
            const auto current_value = maze[current.y][current.x];
            const auto neighb = maze::get_neighbours(current, maze, current_value+1);

            if(neighb.values.empty())
                return pos::bad();

            return neighb.values[0];
        }

        template <size_t w, size_t h>
        constexpr auto create_bad_path_message()
        {
            max_array<char, w*h> path{};

            path.push('p');
            path.push('a');
            path.push('t');
            path.push('h');
            path.push(' ');
            path.push('d');
            path.push('o');
            path.push('e');
            path.push('s');
            path.push('n');
            path.push('\'');
            path.push('t');
            path.push(' ');
            path.push('e');
            path.push('x');
            path.push('i');
            path.push('s');
            path.push('t');

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
        constexpr auto extract_path(pos current,
                                      pos to,
                                      maze_array<w,h>& current_maze)
        {
            max_array<char, w*h> path{};

            while(current != to)
            {
                if(current.is_bad())
                    return create_bad_path_message<w,h>();

                const auto next = find_next_path_tile(current, current_maze);
                const char dir = direction(current, next);

                if(dir == '\0')
                    return create_bad_path_message<w,h>();

                path.push(dir);

                current = next;
            }

            return path;
        };
    }



    template <size_t w, size_t h, size_t hash>
    constexpr auto find_path(pos from, pos to)
    {
        maze_array<w, h> maze{};
        maze = disassemble_hash<w, h, hash>();

        details::find_path_impl(from, to, maze);

        return details::extract_path(from, to, maze);
    };
}