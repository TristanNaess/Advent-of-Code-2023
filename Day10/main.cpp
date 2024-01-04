#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

std::size_t flood_recursive(std::size_t x, std::size_t y, std::vector<std::string>& map)
{
	if (map[y][x] == 'X') return 0;
	std::size_t sum = 1;
	map[y][x] = 'X';
	if (y != 0) sum += flood_recursive(x, y-1, map);
	if (y != map.size()-1) sum += flood_recursive(x, y+1, map);
	if (x != 0) sum += flood_recursive(x-1, y, map);
	if (x != map.front().size()-1) sum += flood_recursive(x+1, y, map);
	return sum;
}

struct Turtle
{
    enum class Direction { North, East, South, West };
    
    Turtle(std::size_t x, std::size_t y, Direction f = Direction::North) : x(x), y(y), facing(f) {  }
    std::size_t x, y;
    std::size_t distance = 0;
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

	// allocate second map
	std::vector<std::string> isolated_map(map.size(), std::string(map[0].size(), '.'));

	Turtle turtle{start_x, start_y};

    switch (connection_options) // NESW
    {
        // change these to the entrance direction like L is west and south
        case 0x0C:  map[start_y][start_x] = 'L'; turtle.facing = Turtle::Direction::South; break;
        case 0x0A:  map[start_y][start_x] = '|'; turtle.facing = Turtle::Direction::South; break;
        case 0x09:  map[start_y][start_x] = 'J'; turtle.facing = Turtle::Direction::South; break;
        case 0x06:  map[start_y][start_x] = 'F'; turtle.facing = Turtle::Direction::West; break;
        case 0x05:  map[start_y][start_x] = '-'; turtle.facing = Turtle::Direction::West; break;
        case 0x03:  map[start_y][start_x] = '7'; turtle.facing = Turtle::Direction::North; break;
        default:
            std::cerr << "Failed to determine shape of 'S' segment\n";
            return EXIT_FAILURE;
    }

	std::size_t bounds_x = start_x;
	std::size_t bounds_y = start_y;
	std::size_t bounds_x2 = start_x;
	std::size_t bounds_y2 = start_y;
	

	// copy loop to second map
    bool first = true;
    while (!(turtle.x == start_x && turtle.y == start_y) || first)
    {
		isolated_map[turtle.y][turtle.x] = 'X';
        first = false;
		if (turtle.move(map) == false)
		{
			std::cerr << "Error. Turtle died\n";
			return EXIT_FAILURE;
        }
		if (bounds_x > turtle.x) bounds_x = turtle.x;
		if (bounds_y > turtle.y) bounds_y = turtle.y;
		if (bounds_x2 < turtle.x) bounds_x2 = turtle.x;
		if (bounds_y2 < turtle.y) bounds_y2 = turtle.y;
    }

	// shrink map to have border of 1
	bounds_x--;
	bounds_x2++;
	bounds_y--;
	bounds_y2++;

	isolated_map.erase(isolated_map.begin() + bounds_y2+1, isolated_map.end()); // remove far bound first
	isolated_map.erase(isolated_map.begin(), isolated_map.begin() + bounds_y);

	for (auto& row : isolated_map)
	{
		if (bounds_x2 < row.size()) row.erase(bounds_x2+1);
		row.erase(0, bounds_x);
	}

	std::size_t sum = isolated_map.size() * isolated_map.front().size();
	sum -= flood_recursive(0, 0, isolated_map);
	sum -= turtle.distance;

	std::cout << "Interior area: " << sum << '\n';
}
