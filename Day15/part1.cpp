#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string_view>
#include <algorithm>

unsigned int hash(unsigned int init, const std::string& str)
{
    unsigned int val = 0;
    for (const char c : str)
    {
        val += c;
        val *= 17;
        val %= 256;
    }

    return val + init;
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

    std::vector<std::string> steps;
    std::string buffer;
    while (std::getline(file, buffer))
    {
        std::istringstream ss{buffer};
        while (std::getline(ss, buffer, ','))
        {
            steps.push_back(buffer);
        }
    }

    unsigned int sum = std::ranges::fold_left(steps, 0, hash);

    std::cout << "Sum: " << sum << '\n';
}
