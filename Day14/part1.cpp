#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Grid
{
    public:
        Grid(std::vector<std::string> data_vec)
        {
            m_height = data_vec.size();
            m_width = data_vec.front().size();
            data = std::vector<char>(m_height * m_width, 0);
            std::size_t i = 0;
            for (const auto& row : data_vec)
            {
                for (const auto& c : row)
                {
                    data[i] = c;
                    i++;
                }
            }
        }

        char& operator[](std::size_t row, std::size_t col) // C++23
        {
            if (row >= m_height) throw std::out_of_range("Requested row greater than height");
            if (col >= m_width) throw std::out_of_range("Requested column greater than width");
            return data[m_width * row + col];
        }

        std::size_t height() const
        {
            return m_height;
        }

        std::size_t width() const
        {
            return m_width;
        }

        void shift_north()
        {
            std::size_t place_index;
            for (std::size_t i = 0; i < m_width; i++)
            {
                std::size_t count = 0;
                for (std::size_t j = m_height - 1; j < m_height; j--)
                {
                    switch(data[m_width * j + i])
                    {
                        case 'O':
                            data[m_width * j + i] = '.';
                            count++;
                            break;
                        case '#':
                            place_index = j+1;
                            while (count > 0)
                            {
                                data[m_width * place_index + i] = 'O';
                                count--;
                                place_index++;
                            }
                            break;
                        case '.':
                            break;
                    }
                }
                place_index = 0;
                while (count > 0)
                {
                    data[m_width * place_index + i] = 'O';
                    count--;
                    place_index++;
                }
            }
        }

        std::size_t calc_weight() const
        {
            std::size_t sum = 0;
            for (std::size_t row = 0; row < m_height; row++)
            {
                for (std::size_t col = 0; col < m_width; col++)
                {
                    if (data[row * m_width + col] == 'O')
                    {
                        sum += m_height - row;
                    }
                }
            }

            return sum;
        }

        friend std::ostream& operator<<(std::ostream& os, const Grid& g);

    private:
        std::vector<char> data;
        std::size_t m_height, m_width;
};

std::ostream& operator<<(std::ostream& os, const Grid& g)
{
    std::size_t col_count = 0;
    for (const auto& c : g.data)
    {
        os << c;
        col_count++;
        if (col_count == g.width())
        {
            os << '\n';
            col_count = 0;
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

    grid.shift_north();

    std::cout << "Weight: " << grid.calc_weight() << '\n';
}
