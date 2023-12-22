#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// returns number of rows above the mirror; returns no of rows if no mirror
std::size_t find_mirror_row(const std::vector<std::vector<char>>& data)
{
    // I know ranges::views::pairwise will work here, but don't have the time to figure it out right now
    for (auto itr = data.begin(); itr < data.end() - 1; itr++)
    {
        if (*itr == *(itr+1))
        {
            bool mirrored = true;
            for (auto l_itr = itr, r_itr = itr+1; l_itr >= data.begin() && r_itr < data.end(); l_itr--, r_itr++)
            {
                if (*l_itr != *r_itr) { mirrored = false; break; }
            }
            if (mirrored) return itr - data.begin() + 1;
        }
    }
    return data.size();
}

// returns number of columns left of the mirror; returns no of columns if no mirror
std::size_t find_mirror_col(const std::vector<std::vector<char>>& data)
{
    std::vector<std::vector<char>> data_transpose(data.front().size(), std::vector<char>(data.size(), ' '));
    for (std::size_t r = 0; r < data.size(); r++)
    {
        for (std::size_t c = 0; c < data.front().size(); c++)
        {
            data_transpose[c][r] = data[r][c];
        }
    }

    return find_mirror_row(data_transpose);
}

int main(int argc, char** argv)
{
    // Arg check
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

    std::vector<std::vector<std::vector<char>>> patterns;
    patterns.push_back(std::vector<std::vector<char>>());

    // Extract Data
    std::string buffer;
    while (std::getline(file, buffer))
    {
        if (buffer.empty())
        {
            patterns.push_back(std::vector<std::vector<char>>());
            continue;
        }
        patterns.back().emplace_back(buffer.begin(), buffer.end());
    }

    file.close();

    // Find mirrors

    std::size_t row_sum = 0, col_sum = 0;
    for (const auto& pattern : patterns)
    {
        std::size_t row = find_mirror_row(pattern);
        if (row == pattern.size())
        {
            col_sum += find_mirror_col(pattern);
            continue;
        }
        row_sum += row;
    }

    std::cout << "Summary val: " << col_sum + (100 * row_sum) << '\n';
}
