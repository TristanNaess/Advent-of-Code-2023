#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class Hand
{
    public:
        Hand(const std::string& cards, int bid) : _cards(cards), _bid(bid) {  }

        int bid() const
        {
            return _bid;
        }

        bool operator>(const Hand& rhs) const
        {
            Type ltype, rtype;
            ltype = rank();
            rtype = rhs.rank();

            if (ltype > rtype) return true;
            if (ltype < rtype) return false;

            static const std::string faces = "23456789TJQKA";

            char lc, rc;
            for (std::size_t i = 0; i < _cards.size(); i++)
            {
                lc = _cards[i];
                rc = rhs._cards[i];
                if (lc == rc) continue;
                if (faces.find(lc) > faces.find(rc)) return true;
                return false;
            }
            return false;

        }

        bool operator==(const Hand& rhs) const
        {
            return _cards == rhs._cards;
        }

        bool operator<(const Hand& rhs) const
        {
            return *this != rhs && !(*this > rhs);
        }

    private:
        
        enum class Type
        {
            Five_Of_Kind = 6,
            Four_Of_Kind = 5,
            Full_House = 4,
            Three_Of_Kind = 3,
            Two_Pair = 2,
            One_Pair = 1,
            High_Card = 0
        };

        Type rank() const
        {
            std::vector<int> counts = {0,0,0,0,0,0,0,0,0,0,0,0,0};
            for (const char& c : _cards)
            {
                switch (c)
                {
                    case '2':
                        counts[0]++;
                        break;
                    case '3':
                        counts[1]++;
                        break;
                    case '4':
                        counts[2]++;
                        break;
                    case '5':
                        counts[3]++;
                        break;
                    case '6':
                        counts[4]++;
                        break;
                    case '7':
                        counts[5]++;
                        break;
                    case '8':
                        counts[6]++;
                        break;
                    case '9':
                        counts[7]++;
                        break;
                    case 'T':
                        counts[8]++;
                        break;
                    case 'J':
                        counts[9]++;
                        break;
                    case 'Q':
                        counts[10]++;
                        break;
                    case 'K':
                        counts[11]++;
                        break;
                    case 'A':
                        counts[12]++;
                        break;
                }
            }

            counts.erase(std::remove(counts.begin(), counts.end(), 0), counts.end());

            if (std::ranges::find(counts, 5) != counts.end()) return Type::Five_Of_Kind;
            if (std::ranges::find(counts, 4) != counts.end()) return Type::Four_Of_Kind;
            if (std::ranges::find(counts, 3) != counts.end())
            {
                if (std::ranges::find(counts, 2) != counts.end()) return Type::Full_House;
                return Type::Three_Of_Kind;
            }
            if (std::ranges::find(counts, 2) != counts.end())
            {
                counts.erase(std::ranges::find(counts, 2));
                if (std::ranges::find(counts, 2) != counts.end()) return Type::Two_Pair;
                return Type::One_Pair;
            }
            return Type::High_Card;
        }

        std::string _cards;
        int _bid;
};

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <Input File>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file{argv[1]};
    if (!file.good())
    {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    std::vector<Hand> hands;

    std::string buffer;
    int bid;

    while (std::getline(file, buffer))
    {
        bid = std::stoi(buffer.substr(buffer.find(' ') + 1));
        buffer = buffer.substr(0, buffer.find(' '));
        hands.emplace_back(buffer, bid);
    }

    std::sort(hands.begin(), hands.end());

    int rank = 1;
    int sum = 0;
    for (const auto& h : hands)
    {
        sum += h.bid() * rank;
        rank++;
    }

    std::cout << "Winnings: " << sum << '\n';
}
