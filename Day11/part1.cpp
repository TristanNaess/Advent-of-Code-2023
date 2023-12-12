#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct Galaxy
{
    Galaxy(std::size_t x, std::size_t y) : x(x), y(y) {  }
    std::size_t x, y;

    std::size_t distance(const Galaxy& other) const
    {
        std::size_t x_dist, y_dist;
        if (x > other.x) x_dist = x - other.x;
        else x_dist = other.x - x;

        if (y > other.y) y_dist = y - other.y;
        else y_dist = other.y - y;

        return x_dist + y_dist;
    }
};


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

    std::vector<std::string> map;

    std::string buffer;
    while (std::getline(file, buffer))
    {
        map.push_back(buffer);
    }
    file.close();

    for (const auto& row : map)
    {
        std::cout << row << '\n';
    }
    std::cout << '\n';

    // expand horizontal
    std::vector<bool> expandable_rows(map[0].size(), true);
    for (std::size_t i = 0; i < map[0].size(); i++)
    {
        for (const auto& r : map)
        {
            if (r[i] == '#')
            {
                expandable_rows[i] = false;
            }
        }
    }
    for (std::size_t i = 0; i < map[0].size(); i++)
    {
        if (expandable_rows[i] == true)
        {
            expandable_rows.insert(expandable_rows.begin() + i, true);
            for (auto& r : map)
            {
                r.insert(i, ".");
            }
            i++;
        }
    }

    // expand vertical
    std::string empty_row(map[0].size(), '.');

    for (auto itr = map.begin(); itr < map.end(); itr++)
    {
        if (*itr == empty_row)
        {
            map.insert(itr, empty_row);
            itr++;
        }
    }


    for (const auto& row : map)
    {
        std::cout << row << '\n';
    }

    std::vector<Galaxy> galaxies;

    for (std::size_t j = 0; j < map.size(); j++)
    {
        for (std::size_t i = 0; i < map[0].size(); i++)
        {
            if (map[j][i] == '#')
            {
                galaxies.emplace_back(i, j);
            }
        }
    }

    std::size_t sum = 0;
    for (auto itr = galaxies.begin(); itr < galaxies.end(); itr++)
    {
        for (auto jtr = itr+1; jtr < galaxies.end(); jtr++)
        {
            sum += (*itr).distance(*jtr);
        }
    }

    std::cout << "Sum of distances: " << sum << '\n';
}
