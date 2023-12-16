#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

struct Turtle
{
    enum class Direction { North, East, South, West };
    
    Turtle(std::size_t x, std::size_t y, Direction f = Direction::North) : x(x), y(y), facing(f) {  }
    std::size_t x, y;
    std::size_t distance = 0; // turtles start one from start
    Direction facing = Direction::North;

    bool move(std::vector<std::string> map)
    {
        char current = map[y][x];
        distance++;

        // I'm not checking for edge of map conditions, that should be done elsewhere if I were to do it
        switch (current)
        {
            case '|':
                if (facing == Direction::North)
                {
                    y--;
                    // facing = Direction::North;
                    break;
                }
                y++;
                // facing = Direction::South;
                break;
            case '-':
                if (facing == Direction::East)
                {
                    x++;
                    // facing = Direction::East;
                    break;
                }
                x--;
                // facing = Direction::West;
                break;
            case 'L':
                if (facing == Direction::South)
                {
                    x++;
                    facing = Direction::East;
                    break;
                }
                y--;
                facing = Direction::North;
                break;
            case 'J':
                if (facing == Direction::South)
                {
                    x--;
                    facing = Direction::West;
                    break;
                }
                y--;
                facing = Direction::North;
                break;
            case '7':
                if (facing == Direction::North)
                {
                    x--;
                    facing = Direction::West;
                    break;
                }
                y++;
                facing = Direction::South;
                break;
            case 'F':
                if (facing == Direction::North)
                {
                    x++;
                    facing = Direction::East;
                    break;
                }
                y++;
                facing = Direction::South;
                break;
            default:
                return false;
        }
        return true;
    }
};

bool operator!=(const Turtle& t1, const Turtle& t2)
{
    return !(t1.x == t2.x && t1.y == t2.y);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <Input File>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file{argv[1]};
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    std::size_t start_x = std::string::npos;
    std::size_t start_y = std::string::npos;
    std::vector<std::string> map;

    std::string buffer;
    std::getline(file, buffer);
    map.push_back(buffer);
    while (std::getline(file, buffer))
    {
        map.push_back(buffer);
        if (buffer.find('S') != std::string::npos)
        {
            start_x = buffer.find('S'); // I'm assuming this will be opitimized away and reused from 2 lines above
            start_y = map.size() - 1;
        }
    }

    if (start_x == std::string::npos || start_y == std::string::npos)
    {
        std::cerr << "No start location located\n";
        return EXIT_FAILURE;
    }

    // figure out what 'S' is
    unsigned char connection_options = 0x00; // directions that are connected to 'S' lower nibble is NESW
    // there has to be a more elegant solution than this...
    if (start_y != 0)
    {
        switch (map[start_y-1][start_x])
        {
            case '|':
            case '7':
            case 'F':
                connection_options += 0x08;
                break;
        }
    }
    if (start_y < map.size()-1)
    {
        switch(map[start_y+1][start_x])
        {
            case '|':
            case 'L':
            case 'J':
                connection_options += 0x02;
                break;
        }
    }
    if (start_x != 0)
    {
        switch (map[start_y][start_x-1])
        {
            case '-':
            case 'L':
            case 'F':
                connection_options += 0x01;
                break;
        }
    }
    if (start_y != map.front().size()-1)
    {
        switch (map[start_y][start_x+1])
        {
            case '-':
            case 'J':
            case '7':
                connection_options += 0x04;
                break;
        }
    }

    std::array<Turtle, 2> turtles = { { {start_x, start_y}, {start_x, start_y} } };

    switch (connection_options) // NESW
    {
        // change these to the entrance direction like L is west and south
        case 0x0C:  map[start_y][start_x] = 'L'; turtles[0].facing = Turtle::Direction::South; turtles[1].facing = Turtle::Direction::West; break;
        case 0x0A:  map[start_y][start_x] = '|'; turtles[0].facing = Turtle::Direction::South; turtles[1].facing = Turtle::Direction::North; break;
        case 0x09:  map[start_y][start_x] = 'J'; turtles[0].facing = Turtle::Direction::South; turtles[1].facing = Turtle::Direction::East; break;
        case 0x06:  map[start_y][start_x] = 'F'; turtles[0].facing = Turtle::Direction::West; turtles[1].facing = Turtle::Direction::North; break;
        case 0x05:  map[start_y][start_x] = '-'; turtles[0].facing = Turtle::Direction::West; turtles[1].facing = Turtle::Direction::East; break;
        case 0x03:  map[start_y][start_x] = '7'; turtles[0].facing = Turtle::Direction::North; turtles[1].facing = Turtle::Direction::East; break;
        default:
            std::cerr << "Failed to determine shape of 'S' segment\n";
            return EXIT_FAILURE;
    }

    // iterate until done
    bool first = true;
    while (turtles[0] != turtles[1] || first == true)
    {
        first = false;
        for (Turtle& t : turtles)
        {
            if (t.move(map) == false)
            {
                std::cerr << "Error. Turtle died\n";
                return EXIT_FAILURE;
            }
        }
    }

    std::cout << "Furthest Distance: " << turtles.front().distance << '\n';
}
