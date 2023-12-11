#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <deque>

class Sequence
{
    public:
        Sequence(const std::string& line)
        {
            std::stringstream ss{line};
            vals.push_back(std::deque<long int>{});

            std::string buffer;
            while (std::getline(ss, buffer, ' '))
            {
                vals.back().push_back(std::stol(buffer));
            }
        }

        void calc_next_val()
        {
            // populate differences until all 0's
            bool all_zeros = false;
            while (!all_zeros)
            {
                auto& prev = vals.back();

                std::deque<long int> differences{};

                all_zeros = true;
                for (auto itr = prev.begin(); itr != prev.end() - 1; itr++)
                {
                    differences.push_back(*(itr+1) - *itr);
                    if (differences.back() != 0) all_zeros = false;
                }
                vals.push_back(std::move(differences));
            }

            for (auto itr = (vals.end()) - 1; itr > vals.begin(); itr--)
            {
                (itr - 1)->push_back((itr - 1)->back() + itr->back());
                (itr - 1)->push_front((itr - 1)->front() - itr->front());
            }
            

        }

        long int last_val() const
        {
            return vals.front().back();
        }

        long int first_val() const
        {
            return vals.front().front();
        }

    private:
        std::vector<std::deque<long int>> vals;

        friend std::ostream& operator<<(std::ostream&, const Sequence&);
};

std::ostream& operator<<(std::ostream& os, const Sequence& seq)
{
    for (const auto& l : seq.vals)
    {
        for (const auto& n : l)
        {
            os << n << ' ';
        }
        os << '\n';
    }
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

    std::vector<Sequence> sequences; 
    std::string buffer;
    while (std::getline(file, buffer))
    {
        sequences.emplace_back(buffer);
        sequences.back().calc_next_val();

        //std::cout << sequences.back() << '\n';
    }

    long int sum_next = std::ranges::fold_left(sequences.cbegin(), sequences.cend(), 0, [](long int sum, const Sequence& s){ sum += s.last_val(); return sum; });
    long int sum_last = std::ranges::fold_left(sequences.cbegin(), sequences.cend(), 0, [](long int sum, const Sequence& s){ sum += s.first_val(); return sum; });

    std::cout << "Sum of extrapolated vals: " << sum_next << "\nSum of extrapolated history: " << sum_last << '\n';;
}
