#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>

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

    std::vector<std::tuple<int, int>> races;
    
    std::string times, distances;
    std::getline(file, times);
    std::getline(file, distances);

    times.erase(0, times.find(':') + 1);
    distances.erase(0, distances.find(':') + 1);

    std::stringstream sst{times};
    std::stringstream ssd{distances};

    int t, d;
    while (sst.good())
    {
        sst >> t;
        ssd >> d;
        races.push_back(std::make_tuple(t, d));
    }

    int distance, win_count, time;
    int margin = 1;
    for (const auto& r : races)
    {
        win_count = 0;
        time = std::get<0>(r);
        for (int i = 0; i < time; i++)
        {
            distance = (time - i) * i;
            if (distance > std::get<1>(r))
            {
                win_count++;
            }
        }
        margin *= win_count;
    }

    std::cout << "Margin of Error: " << margin << '\n';
}
