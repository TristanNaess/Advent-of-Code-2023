#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <cstdint>

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

    std::string time_input, distance_input;
    std::getline(file, time_input);
    std::getline(file, distance_input);

    time_input.erase(0, time_input.find(':') + 1);
    distance_input.erase(0, distance_input.find(':') + 1);

    std::string time_s, distance_s;
    time_s.reserve(time_input.size());
    distance_s.reserve(distance_input.size());

    std::copy_if(time_input.begin(), time_input.end(), std::back_inserter(time_s), [](char c){ return c != ' '; });
    std::copy_if(distance_input.begin(), distance_input.end(), std::back_inserter(distance_s), [](char c){ return c != ' '; });

    uint64_t time = std::stol(time_s);
    uint64_t distance = std::stol(distance_s);

    uint64_t win_count = 0;
    uint64_t temp_dist;
    for (uint64_t i = 0; i < time; i++)
    {
        temp_dist = (time - i) * i;
        if (temp_dist > distance)
        {
            win_count++;
        }
    }

    std::cout << "Number of ways to win: " << win_count << '\n';
}
