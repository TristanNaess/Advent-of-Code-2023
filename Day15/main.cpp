#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>


struct Lens
{
    Lens() : label(""), op('\0'), focal_length(0) {  }

    Lens (const std::string& code)
    {
        std::size_t delim = code.find_first_of("-=");
        op = code[delim];
        label = code.substr(0, delim);
        if (op == '=') { focal_length = std::stoul(code.substr(delim+1)); }
    }

    std::string label;
    char op;
    unsigned int focal_length;
};

bool operator==(const Lens& l1, const Lens& l2)
{
    return (l1.label == l2.label);
}

unsigned int hash(const std::string& str)
{
    unsigned int val = 0;
    for (const char c : str)
    {
        val += c;
        val *= 17;
        val %= 256;
    }

    return val;
}

class Hashmap
{
    public:
        Hashmap() {}

        void remove(const Lens& l)
        {
            std::size_t index = hash(l.label);
            std::vector<Lens>& box = boxes[index];
            auto itr = std::find(box.begin(), box.end(), l);
            if (itr == box.end()) { return; }
            box.erase(itr);
        }

        void add(const Lens& l)
        {
            std::size_t index = hash(l.label);
            std::vector<Lens>& box = boxes[index];
            auto itr = std::find(box.begin(), box.end(), l);
            if (itr == box.end()) { box.push_back(l); return; }
            *itr = l;
        }

        unsigned long int power() const
        {
            unsigned long int power = 0;
            std::size_t box_i = 1, slot_i;
            for (const auto& b : boxes)
            {
                slot_i = 1;
                for (const auto& l : b)
                {
                    power += l.focal_length * box_i * slot_i;
                    slot_i++;
                }
                box_i++;
            }

            return power;
        }

        friend std::ostream& operator<<(std::ostream& os, const Hashmap& h);

    private:
        std::array<std::vector<Lens>, 255> boxes;
};

std::ostream& operator<<(std::ostream& os, const Hashmap& h)
{
    std::size_t i = 0;
    for (const auto& b : h.boxes)
    {
        if (b.size() != 0)
        {
            os << "Box " << i << ": ";
            for (const auto l : b)
            {
                os << '[' << l.label << ' ' << l.focal_length << "] ";
            }
            os << '\n';
        }
        i++;
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

    Hashmap boxes;

    std::vector<std::string> steps;
    std::string buffer;
    while (std::getline(file, buffer))
    {
        std::istringstream ss{buffer};
        while (std::getline(ss, buffer, ','))
        {
            Lens l{buffer};
            switch (l.op)
            {
                case '-':
                    boxes.remove(l);
                    break;
                case '=':
                    boxes.add(l);
                    break;
            }

            //std::cout << buffer << ":\n" << boxes << '\n';
        }
    }

    file.close();

    std::cout << "Power: " << boxes.power() << '\n';
}
