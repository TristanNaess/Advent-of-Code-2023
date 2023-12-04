#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Game
{
    public:
        Game() : _id(-1), red(0), green(0), blue(0) {  }
        Game(int r, int g, int b) : _id(-1), red(r), green(g), blue(b) {  }
        Game(const std::string& line)
        {
            std::size_t start = 5; // skip "Game "
            std::size_t end = line.find(':');
            _id = std::stoi(line.substr(start, end - start));

            red = 0;
            green = 0;
            blue = 0;

            static std::string digits = "0123456789";
            int accumulator = 0;
            for (auto itr = line.begin() + end; itr != line.end(); itr++)
            {
                if (digits.contains(*itr)) // C++23
                {
                    accumulator = accumulator * 10 + (*itr - '0');
                }
                else
                {
                    switch(*itr)
                    {
                        case 'r': // this will technically catch the r in green, but accumulator will be 0 at the time, so its just slower
                            if (red < accumulator) red = accumulator;
                            accumulator = 0;
                            break;
                        case 'g':
                            if (green < accumulator) green = accumulator;
                            accumulator = 0;
                            break;
                        case 'b':
                            if (blue < accumulator) blue = accumulator;
                            accumulator = 0;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        bool fits_in(const Game& other)
        {
            return (red <= other.red && blue <= other.blue && green <= other.green);
        }

        int power() const
        {
            return red*green*blue;
        }

        int id() { return _id; }

        friend std::ostream& operator<<(std::ostream&, const Game&);

    private:
        int _id, red, green, blue;
};


std::ostream& operator<<(std::ostream& os, const Game& g)
{
    os << '[' << g._id << "] " << g.red << "r, " << g.green << "g, " << g.blue << 'b';
    return os;
}


int main(int argc, char** argv)
{
    if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file{argv[1]};
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }


    int sum = 0;
    Game max{12, 13, 14};
    Game game;
    std::string buffer;
    while (std::getline(file, buffer))
    {
        //std::cout << buffer << '\n';
        game = Game{buffer};
        //std::cout << game << '\n';
        /*
        if (game.fits_in(max))
        {
            sum += game.id();
        }
        */
        sum += game.power();
    }

    std::cout << "Sum of IDs: " << sum << '\n';

}
