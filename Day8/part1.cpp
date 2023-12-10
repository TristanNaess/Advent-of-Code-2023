#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstdint>

enum class Direction { Left, Right };

class Graph
{
    public:
        Graph() {  }

        void add_node(const std::string& line)
        {
            std::string name, left, right;
            name = line.substr(0, 3);
            left = line.substr(7, 3);
            right = line.substr(12, 3);

            Node& n = nodes[name]; // insert is intentional here
            n.left_key = left;
            n.right_key = right;
            
            nodes[left];  // insert if missing with children as ""
            nodes[right];
        }

        std::string next_node(const std::string& current, Direction d)
        {
            try
            {
                Node& n = nodes.at(current);
                switch (d)
                {
                    case Direction::Left:
                        return n.left_key;
                        break;
                    case Direction::Right:
                        return n.right_key;
                        break;
                    default:
                        std::cerr << "Somehow gave bad direction\n";
                        exit(EXIT_FAILURE);
                }
            }
            catch (const std::out_of_range& e)
            {
                std::cerr << "No node named \"" << current << "\"\n";
                exit(EXIT_FAILURE);
            }
        }

    private:
        struct Node
        {
            std::string left_key = "";
            std::string right_key = "";
        };

        std::unordered_map<std::string, Node> nodes;
};


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[1] << " <Input File>\n";
        return EXIT_FAILURE;
    }

    std::ifstream file{argv[1]};
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    Graph map;

    std::string directions;
    std::getline(file, directions);

    std::string buffer;
    while (std::getline(file, buffer))
    {
        if (buffer.empty()) continue;

        map.add_node(buffer);
    }

    uint32_t steps = 0;
    std::string current = "AAA";
    Direction dir;
    std::size_t dir_len = directions.size();
    while (current != "ZZZ")
    {
        if (directions[steps % dir_len] == 'L') dir = Direction::Left;
        else dir = Direction::Right;

        steps++;
        current = map.next_node(current, dir);
    }

    std::cout << "Completed in " << steps << " steps\n";
}
