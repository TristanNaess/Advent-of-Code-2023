#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <limits>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " [Input file]" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream input{argv[1]};
    if (!input.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int sum = 0;
    std::size_t min_index, max_index, temp_index;
    std::size_t min_string_index, max_string_index;
    std::array<std::string, 18> substrings = { "one", "two", "three", "four", "five", "six", "seven", "eight","nine", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    std::string line;
    while (std::getline(input, line))
    {
        min_index = std::numeric_limits<std::size_t>::max();
        max_index = 0;
        min_string_index = 0;
        max_string_index = 0;
        for (std::size_t i = 0; i < 18; i++)
        {
            temp_index = line.find(substrings[i]);
            if (temp_index <= min_index)
            {
                min_index = temp_index;
                min_string_index = i;
            }
            temp_index = line.rfind(substrings[i]);
            if (temp_index >= max_index && temp_index != std::string::npos)
            {
                max_index = temp_index;
                max_string_index = i;
            }

        }
        
        std::cout << "values are: " << substrings[min_string_index] << ' ' << substrings[max_string_index] << ' ' << '(' << (min_string_index % 9) + 1 << (max_string_index % 9) + 1 << ')' << line << '\n';

        sum += ((min_string_index % 9) + 1) * 10;
        sum += (max_string_index % 9) + 1;

    }

    std::cout << "The sum calibration value is: " << sum << std::endl;
}
