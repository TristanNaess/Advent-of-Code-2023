#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>

class Card
{
    public:
        Card() {  };
        Card(const std::string& line)
        {
            std::string winning_string = line.substr(0, line.find('|') - 1); // don't include space before |
            winning_string.erase(0, winning_string.find(':') + 1); // remove card number and initial space
            std::string your_string = line.substr(line.find('|') + 1);

            std::size_t index;
            while (winning_string.size() != 0)
            {
                winning_numbers.push_back(std::stoi(winning_string.substr(winning_string.size() - 2)));
                winning_string.erase(winning_string.size() - 3, 3);
            }

            while (your_string.size() != 0)
            {
                your_numbers.push_back(std::stoi(your_string.substr(your_string.size() - 2)));
                your_string.erase(your_string.size() - 3, 3);
            }

            std::sort(winning_numbers.begin(), winning_numbers.end());
            std::sort(your_numbers.begin(), your_numbers.end());
        }

        int score()
        {
            std::vector<int> matches;
            std::set_intersection(winning_numbers.cbegin(), winning_numbers.cend(), your_numbers.cbegin(), your_numbers.cend(), std::back_inserter(matches));
            if (matches.size() == 0) return 0;
            return 1 << matches.size() - 1;
        }

        friend std::ostream& operator<<(std::ostream&, const Card&);

    private:
        std::vector<int> winning_numbers;
        std::vector<int> your_numbers;
};

std::ostream& operator<<(std::ostream& os, const Card& c)
{
    std::copy(c.winning_numbers.cbegin(), c.winning_numbers.cend(), std::ostream_iterator<int>(os, " "));
    std::cout << " | ";
    std::copy(c.your_numbers.cbegin(), c.your_numbers.cend(), std::ostream_iterator<int>(os, " "));
    return os;
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

    int sum = 0;
    std::string buffer;
    while (std::getline(file, buffer))
    {
        Card c{buffer};
        sum += c.score();
    }
    
    std::cout << "Sum of scores: " << sum << '\n';

}
