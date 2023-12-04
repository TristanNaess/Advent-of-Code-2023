#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
    int ratio = 1;
    int adjacent;
    char c;
    std::size_t start, end;
    for (std::size_t row = 0; row < data.size(); row++)
    {
        for (std::size_t col = 0; col < data[0].size(); col++) // data is square, so specific row doesn't matter
        {
            c = data[row][col];
            if (c == '*')
            {
                adjacent = 0;
                ratio = 1;
                if (row != 0)
                {
                    if (col != 0 && is_digit(data[row-1][col-1]))                   { ratio *= extract_number(data[row-1], col-1); adjacent++; } // upper left
                    if (is_digit(data[row-1][col]))                                 { ratio *= extract_number(data[row-1], col);   adjacent++; } // up
                    if (col != data[0].size() - 1 && is_digit(data[row-1][col+1]))  { ratio *= extract_number(data[row-1], col+1); adjacent++; } // upper right
                }
                    if (col != 0 && is_digit(data[row][col-1]))                     { ratio *= extract_number(data[row], col-1);   adjacent++; } // left
                    if (col != data[0].size() - 1 && is_digit(data[row][col+1]))    { ratio *= extract_number(data[row], col+1);   adjacent++; } // right
                if (row != data.size() - 1)
                {
                    if (col != 0 && is_digit(data[row+1][col-1]))                   { ratio *= extract_number(data[row+1], col-1); adjacent++; } // lower left
                    if (is_digit(data[row+1][col]))                                 { ratio *= extract_number(data[row+1], col);   adjacent++; } // down
                    if (col != data[0].size() - 1 && is_digit(data[row+1][col+1]))  { ratio *= extract_number(data[row+1], col+1); adjacent++; } // lower right
                }
                if (adjacent == 2) sum += ratio;
            }
        }
    }

    std::cout << "Sum of numbers: " << sum << "\n";
}
