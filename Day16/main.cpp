#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Grid
{
    public:
        Grid(const std::vector<std::string>& data)
        {
            m_height = data.size();
            m_width = data.front().size();

            m_data.reserve(m_height * m_width);

            for (const auto& row : data)
            {
                for (const char& c : row)
                {
                    m_data.emplace_back(c);
                }
            }
        }

    private:

        enum class Direction : unsigned char { Up, Down, Left, Right };

        struct Cell
        {
            Cell(const char c)
            {
                hot = false;
                switch (c)
                {
                    case '.':  type = Type::Empty;   break;
                    case '/':  type = Type::RMirror; break;
                    case '\\': type = Type::LMirror; break;
                    case '|':  type = Type::VSplit;  break;
                    case '-':  type = Type::HSplit;  break;
                }
            }

            enum class Type { Empty, RMirror, LMirror, VSplit, HSplit } type;
            bool hot;
            Direction accessed;
        };

        std::size_t m_width, m_height;
        std::vector<Cell> m_data;

        friend std::ostream& operator<<(std::ostream& os, const Grid& g);
};

std::ostream& operator<<(std::ostream& os, const Grid& g)
{
    std::size_t count = 0;
    for (const Grid::Cell& c : g.m_data)
    {
        if (c.hot) os << "\033[7m";

        switch (c.type)
        {
            case Grid::Cell::Type::Empty:   os << '.';  break;
            case Grid::Cell::Type::RMirror: os << '/';  break;
            case Grid::Cell::Type::LMirror: os << '\\'; break;
            case Grid::Cell::Type::VSplit:  os << '|';  break;
            case Grid::Cell::Type::HSplit:  os << '-';  break;
        }
        os << "\033[0m";

        count++;
        if (count == g.m_width)
        {
            os << '\n';
            count = 0;
        }
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

    std::vector<std::string> data;

    std::string buffer;
    while (std::getline(file, buffer))
    {
        data.push_back(buffer);
    }

    Grid grid(data);

    std::cout << grid << '\n';

}
