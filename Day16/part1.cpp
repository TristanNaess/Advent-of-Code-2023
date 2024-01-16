#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <ranges>

class Grid
{
    private:
        enum class Direction : unsigned char { None = 0x00, Up = 0x01, Down = 0x02, Left = 0x04, Right = 0x08 };

        friend Direction& operator|=(Direction& d1, const Direction& d2);
        friend bool operator&(const Direction& d1, const Direction& d2);

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
            Direction accessed = Direction::None;
        };

        struct Beam
        {
            std::size_t x, y;
            Direction dir;
        };

        std::size_t m_width, m_height;
        std::vector<Cell> m_data;

        std::queue<Beam> beams;

        friend std::ostream& operator<<(std::ostream& os, const Grid& g);

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

        Cell& at(std::size_t x, std::size_t y)
        {
            if (x >= m_width || y > m_height)
                throw std::out_of_range("One or more indexes out of bounds");
            return m_data[m_width * y + x];
        }

        std::size_t activate()
        {
            beams.push(Beam{ .x = 0, .y = 0, .dir = Direction::Right });

            while (!beams.empty())
            {
                Beam& b = beams.front();
                if (b.x >= m_width || b.y >= m_height)
                {
                    beams.pop();
                    continue;
                }

                Cell& cell = at(b.x, b.y);
                cell.hot = true;
                switch (cell.type)
                {
                    case Cell::Type::Empty:
                        switch (b.dir)
                        {
                            case Direction::Up:
                                if (cell.accessed & Direction::Up) break;
                                cell.accessed |= Direction::Up;
                                beams.push(Beam{ .x = b.x, .y = b.y-1, .dir = b.dir });
                                break;
                            case Direction::Down:
                                if (cell.accessed & Direction::Down) break;
                                cell.accessed |= Direction::Down;
                                beams.push(Beam{ .x = b.x, .y = b.y+1, .dir = b.dir });
                                break;
                            case Direction::Left:
                                if (cell.accessed & Direction::Left) break;
                                cell.accessed |= Direction::Left;
                                beams.push(Beam{ .x = b.x-1, .y = b.y, .dir = b.dir });
                                break;
                            case Direction::Right:
                                if (cell.accessed & Direction::Right) break;
                                cell.accessed |= Direction::Right;
                                beams.push(Beam{ .x = b.x+1, .y = b.y, .dir = b.dir });
                                break;
                        };
                        break;
                    case Cell::Type::RMirror:
                        switch (b.dir)
                        {
                            case Direction::Up:
                                if (cell.accessed & Direction::Up) break;
                                cell.accessed |= Direction::Up;
                                beams.push(Beam{ .x = b.x+1, .y = b.y, .dir = Direction::Right });
                                break;
                            case Direction::Down:
                                if (cell.accessed & Direction::Down) break;
                                cell.accessed |= Direction::Down;
                                beams.push(Beam{ .x = b.x-1, .y = b.y, .dir = Direction::Left });
                                break;
                            case Direction::Left:
                                if (cell.accessed & Direction::Left) break;
                                cell.accessed |= Direction::Left;
                                beams.push(Beam{ .x = b.x, .y = b.y+1, .dir = Direction::Down });
                                break;
                            case Direction::Right:
                                if (cell.accessed & Direction::Right) break;
                                cell.accessed |= Direction::Right;
                                beams.push(Beam{ .x = b.x, .y = b.y-1, .dir = Direction::Up });
                                break;
                        };
                        break;
                    case Cell::Type::LMirror:
                        switch (b.dir)
                        {
                            case Direction::Up:
                                if (cell.accessed & Direction::Up) break;
                                cell.accessed |= Direction::Up;
                                beams.push(Beam{ .x = b.x-1, .y = b.y, .dir = Direction::Left });
                                break;
                            case Direction::Down:
                                if (cell.accessed & Direction::Down) break;
                                cell.accessed |= Direction::Down;
                                beams.push(Beam{ .x = b.x+1, .y = b.y, .dir = Direction::Right });
                                break;
                            case Direction::Left:
                                if (cell.accessed & Direction::Left) break;
                                cell.accessed |= Direction::Left;
                                beams.push(Beam{ .x = b.x, .y = b.y-1, .dir = Direction::Up });
                                break;
                            case Direction::Right:
                                if (cell.accessed & Direction::Right) break;
                                cell.accessed |= Direction::Right;
                                beams.push(Beam{ .x = b.x, .y = b.y+1, .dir = Direction::Down });
                                break;
                        };
                        break;
                    case Cell::Type::VSplit:
                        switch (b.dir)
                        {
                            case Direction::Up:
                                if (cell.accessed & Direction::Up) break;
                                cell.accessed |= Direction::Up;
                                beams.push(Beam{ .x = b.x, .y = b.y-1, .dir = Direction::Up });
                                break;
                            case Direction::Down:
                                if (cell.accessed & Direction::Down) break;
                                cell.accessed |= Direction::Down;
                                beams.push(Beam{ .x = b.x, .y = b.y+1, .dir = Direction::Down });
                                break;
                            case Direction::Left:
                                if (cell.accessed & Direction::Left) break;
                                cell.accessed |= Direction::Left;
                                beams.push(Beam{ .x = b.x, .y = b.y-1, .dir = Direction::Up });
                                beams.push(Beam{ .x = b.x, .y = b.y+1, .dir = Direction::Down });
                                break;
                            case Direction::Right:
                                if (cell.accessed & Direction::Right) break;
                                cell.accessed |= Direction::Right;
                                beams.push(Beam{ .x = b.x, .y = b.y-1, .dir = Direction::Up });
                                beams.push(Beam{ .x = b.x, .y = b.y+1, .dir = Direction::Down });
                                break;
                        };
                        break;
                    case Cell::Type::HSplit:
                        switch (b.dir)
                        {
                            case Direction::Up:
                                if (cell.accessed & Direction::Up) break;
                                cell.accessed |= Direction::Up;
                                beams.push(Beam{ .x = b.x-1, .y = b.y, .dir = Direction::Left });
                                beams.push(Beam{ .x = b.x+1, .y = b.y, .dir = Direction::Right });
                                break;
                            case Direction::Down:
                                if (cell.accessed & Direction::Down) break;
                                cell.accessed |= Direction::Down;
                                beams.push(Beam{ .x = b.x-1, .y = b.y, .dir = Direction::Left });
                                beams.push(Beam{ .x = b.x+1, .y = b.y, .dir = Direction::Right });
                                break;
                            case Direction::Left:
                                if (cell.accessed & Direction::Left) break;
                                cell.accessed |= Direction::Left;
                                beams.push(Beam{ .x = b.x-1, .y = b.y, .dir = Direction::Left });
                                break;
                            case Direction::Right:
                                if (cell.accessed & Direction::Right) break;
                                cell.accessed |= Direction::Right;
                                beams.push(Beam{ .x = b.x+1, .y = b.y, .dir = Direction::Right });
                                break;
                        };
                        break;
                };

                beams.pop();
            }

            return std::ranges::fold_left(m_data, 0, [](std::size_t sum, const Cell& c){ if (c.hot) sum += 1; return sum; });
        }
};


Grid::Direction& operator|=(Grid::Direction& d1, const Grid::Direction& d2)
{
    unsigned char val = static_cast<unsigned char>(d1) | static_cast<unsigned char>(d2);
    d1 = static_cast<Grid::Direction>(val);
    return d1;
}

bool operator&(const Grid::Direction& d1, const Grid::Direction& d2)
{
    return static_cast<unsigned char>(d1) & static_cast<unsigned char>(d2);
}

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

    std::size_t sum = grid.activate();

    std::cout << "Number energized: " << sum << '\n';

}
