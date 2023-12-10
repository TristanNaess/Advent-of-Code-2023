#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <algorithm>


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

        std::vector<std::string> node_names()
        {
            std::vector<std::string> names;
            names.reserve(nodes.size());

            for (const auto& n : nodes)
            {
                names.push_back(n.first);
            }

            return names; // This should move be default?
        }

    private:
        struct Node
        {
            std::string left_key = "";
            std::string right_key = "";
        };

        std::unordered_map<std::string, Node> nodes;
};

uint32_t gcd(uint32_t x, uint32_t y)
{
    if (x == 0 || y == 0) return 0;
    if (x == y) return x;
    if (x > y) return gcd(x-y, y);
    return gcd(x, y-x);
}



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


    std::vector<std::string> current_nodes = map.node_names();
    current_nodes.erase(std::remove_if(current_nodes.begin(), current_nodes.end(), [](std::string& name) { return name.back() != 'A'; } ), current_nodes.end());
    std::vector<uint32_t> steps(current_nodes.size(), 0);

    Direction dir;
    std::size_t dir_len = directions.size();

    std::size_t i = 0;
    for (const auto& n : current_nodes)
    {
        while (n.back() != 'Z')
        {
            if (directions[steps[i] % dir_len] == 'L') dir = Direction::Left;
            else dir = Direction::Right;

            steps[i]++;
            current_nodes[i] = map.next_node(n, dir);
        }
        i++;
    }

    uint32_t gcd_val = std::ranges::fold_left(steps.begin() + 1, steps.end(), *steps.begin(), [](uint32_t x, uint32_t y){ return gcd(x, y); });
    uint32_t product = std::ranges::fold_left(steps.begin(), steps.end(), 1, std::multiplies<uint32_t>());

    std::cout << "Completed in " << product/gcd_val << " steps\n";
}
