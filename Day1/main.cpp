#include <iostream>
#include <fstream>
#include <string>

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
    std::size_t index;

    const std::string digits = "0123456789";

    std::string line;
    while (std::getline(input, line))
    {
        index = line.find_first_of(digits);
        if (index == std::string::npos)
        {
            std::cerr << "Error: No digit in line: " << line << std::endl;
            return EXIT_FAILURE;
        }
        sum += (line[index] - '0') * 10;

        index = line.find_last_of(digits);
        // no need to check, there is at least one if we reached here
        sum += (line[index] - '0');
    }

    std::cout << "The sum calibration value is: " << sum << std::endl;
}
