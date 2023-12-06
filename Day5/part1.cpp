#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstdint>

class Mapping
{
    public:
        Mapping() {  }
    
        void add(const std::string& line)
        {
            std::stringstream ss{line};
            mappings.emplace_back();
            ss >> mappings.back().output_start >> mappings.back().input_start >> mappings.back().input_end;

            mappings.back().input_end = mappings.back().input_start + mappings.back().input_end - 1; // last arg is range, just reusing a variable rather than creating another
        }

        uint32_t operator[](uint32_t input) const
        {
            for (const Range& r : mappings)
            {
                if (r.contains(input))
                {
                    return r.output_start + (input - r.input_start);
                }
            }
            return input; // if not contained in a specified range, input = output
        }

    private:
        struct Range
        {
            Range() : input_start(0), input_end(0), output_start(0) {  }

            bool contains(uint32_t val) const
            {
                return val >= input_start && val <= input_end;
            }

            uint32_t input_start;
            uint32_t input_end;
            uint32_t output_start;
        };

        std::vector<Range> mappings;

        friend std::ostream& operator<<(std::ostream&, const Mapping&);
};

std::ostream& operator<<(std::ostream& os, const Mapping& m)
{
    for (const auto i : m.mappings)
    {
        os << i.input_start << '-' << i.input_end << " -> " << i.output_start << '-' << i.output_start + (i.input_end - i.input_start) << '\n';
    }
    return os;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file{argv[1]};
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    std::vector<uint32_t> seeds;
    std::vector<Mapping> mappings;


    std::string buffer;
    while (std::getline(file, buffer))
    {
        if (buffer.empty()) continue;

        if (buffer.find("seeds: ") == 0)
        {
            buffer.erase(0, 7);
            uint32_t s;
            std::istringstream ss{buffer};
            while (ss.good())
            {
                ss >> s;
                seeds.push_back(s);
            }
            continue;
        }

        if (buffer == "seed-to-soil map:" || 
            buffer == "soil-to-fertilizer map:" ||
            buffer == "fertilizer-to-water map:" ||
            buffer == "water-to-light map:" ||
            buffer == "light-to-temperature map:" ||
            buffer == "temperature-to-humidity map:" ||
            buffer == "humidity-to-location map:")
        {
            mappings.emplace_back();
            continue;
        }

        mappings.back().add(buffer);
    }

    for (auto& s : seeds)
    {
        for (const auto& m : mappings)
        {
            s = m[s];
        }
    }

    std::sort(seeds.begin(), seeds.end());

    std::cout << "Minimum location number: " << seeds[0] << '\n';
}
