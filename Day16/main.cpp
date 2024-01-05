#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Grid
{
    Grid(const std::vector<std::string>& data)
    {
        width = data.front().size();
        height = data.size();
        m_data.reserve(width * height);
        for (const auto& l : data)
        {
            for (const char c : l)
            {
                m_data.push_back(c);
            }
        }
    }

    char operator[](std::size_t x, std::size_t y)
    {
        if (x > m_width || y > m_height) throw std::out_of_bounds("Coordinate is outside the grid");
        return m_data[y*m_width + x];
    }

    std::vector<char> m_data;
    std::size_t m_width, m_height;
};

struct Turtle
{
    enum class Direction { Up, Down, Left, Right };

    std::size_t x, y;
    Direction facing;

    bool alive = true;
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

    Grid grid{data};

    std::vector<Turtle> beams{ {.x = 0, .y = 0, .facing = Turtle::Direction::Right, .alive = true} };

    while (beams.size() > 0)
    {
        for (Turtle b : beams)
        {
            // turtle move logic here
        }
    }

}
