#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool is_special_char(const char c)
{
    static std::string non_special_characters = ".0123456789";
    return !non_special_characters.contains(c); // C++23
}

static std::string digits = "0123456789";
bool is_digit(const char c)
{
    return digits.contains(c); // C++23
}

int extract_number(std::string& line, std::size_t index)
{
    std::size_t start = line.find_last_not_of(digits, index);
    if (start == std::string::npos) start = 0;
    else start++;
    std::size_t end = line.find_first_not_of(digits, index);
    if (end == std::string::npos) end = 0;

    int val = std::stoi(line.substr(start, end - start));
    for (; start < end; start++)
    {
        line[start] = '.';
    }
    return val;
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

    std::vector<std::string> data;
    
    std::string buffer;
    while (std::getline(file, buffer))
    {
        data.push_back(buffer);
    }


    int sum = 0;
    char c;
    std::size_t start, end;
    for (std::size_t row = 0; row < data.size(); row++)
    {
        for (std::size_t col = 0; col < data[0].size(); col++) // data is square, so specific row doesn't matter
        {
            c = data[row][col];
            if (is_special_char(c))
            {
                if (row != 0)
                {
                    if (col != 0 && is_digit(data[row-1][col-1]))                   sum += extract_number(data[row-1], col-1); // upper left
                    if (is_digit(data[row-1][col]))                                 sum += extract_number(data[row-1], col);   // up
                    if (col != data[0].size() - 1 && is_digit(data[row-1][col+1]))  sum += extract_number(data[row-1], col+1); // upper right
                }
                    if (col != 0 && is_digit(data[row][col-1]))                     sum += extract_number(data[row], col-1);   // left
                    if (col != data[0].size() - 1 && is_digit(data[row][col+1]))    sum += extract_number(data[row], col+1);   // right
                if (row != data.size() - 1)
                {
                    if (col != 0 && is_digit(data[row+1][col-1]))                   sum += extract_number(data[row+1], col-1); // lower left
                    if (is_digit(data[row+1][col]))                                 sum += extract_number(data[row+1], col);   // down
                    if (col != data[0].size() - 1 && is_digit(data[row+1][col+1]))  sum += extract_number(data[row+1], col+1); // lower right
                }
            }
        }
    }

    for (const auto row : data)
    {
        std::cout << row << '\n';
    }

    std::cout << "Sum of numbers: " << sum << "\n";
}
