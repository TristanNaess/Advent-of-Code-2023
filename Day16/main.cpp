#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Grid
{
    public:
        Grid(const std::vector<std::string>& data);

        std::size_t activate()
        {
            // add beam{0,0,Right} to queue
            // for each beam check cell, make hot, close paths, add next step to queue

            // return number of hot cells
            return 0;
        }

    private:
        struct Cell
        {
            bool hot;
            // See if there's a better way to store directions, that can be reused in Beam
            struct Dirs { bool Up = false, Down = false, Left = false, Right = false; };

            Dirs closed_paths;
        };

        std::vector<Cell> cells;

        struct Beam
        {
            std::size_t x, y;
            // store direction somehow
        };
};

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

}
