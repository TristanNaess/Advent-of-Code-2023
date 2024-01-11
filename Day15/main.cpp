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
        if (op == '=') { focal_length = code[delim+1] - '0'; }
    }

    std::string label = "";
    char op = '\0';
    unsigned int focal_length = 0;
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

// for whatever reason, std::find() would segfault like 50% of the time
std::vector<Lens>::iterator find(std::vector<Lens>& vec, const Lens& l)
{
    for (auto itr = vec.begin(); itr < vec.end(); itr++)
    {
        if (*itr == l) return itr;
    }
    return vec.end();
}

class Hashmap
{
    public:
        Hashmap() {}

        void remove(const Lens& l)
        {
            std::size_t index = hash(l.label);
            std::vector<Lens>& box = boxes[index];
            auto itr = find(box, l);
            //auto itr = std::find(box.begin(), box.end(), l);
            if (itr == box.end()) { return; }
            box.erase(itr);
        }

        void add(const Lens& l)
        {
            std::size_t index = hash(l.label);
            std::vector<Lens>& box = boxes[index];
            auto itr = find(box, l);
            //auto itr = std::find(box.begin(), box.end(), l);
            if (itr == box.end()) { box.push_back(l); return; }
            itr->focal_length = l.focal_length;
        }

        // for debugging, not used in solution
        void insert(const Lens& l)
        {
            std::size_t index = hash(l.label);
            std::vector<Lens>& box = boxes[index];
            box.push_back(l);
        }

        /*
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
        */
        unsigned long int power() const
        {
            unsigned long int power = 0;
            for (std::size_t box = 0; box < boxes.size(); box++)
            {
                for (std::size_t slot = 0; slot < boxes[box].size(); slot++)
                {
                    power += boxes[box][slot].focal_length * (box+1) * (slot+1);
                }
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
                os << '[' << l.label << ' ';
                if (l.op == '-')
                {
                    os << "-]";
                }
                else
                {
                    os << l.focal_length << "] ";
                }
                //os << '[' << l.label << ' ' << l.focal_length << "] ";
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

    std::getline(file, buffer);
    file.close();
    
    std::istringstream ss{buffer};
    
    while (std::getline(ss, buffer, ','))
    {
        Lens l{buffer};
        /*
        std::cout << l.label << ' ' << l.op;
        if (l.focal_length != 0) std::cout << ' ' << l.focal_length;
        std::cout << '\n';
        */

        //*
        switch (l.op)
        {
            case '-':
                boxes.remove(l);
                break;
            case '=':
                boxes.add(l);
                break;
        }
        //*/

        /*
        std::cout << "\033[2J" << buffer << ":\n" << boxes << '\n';
        getchar();
        */

        // just add them all to the hashmap
        //boxes.insert(l);

    }
    //std::cout << boxes << '\n';

    std::cout << "Power: " << boxes.power() << '\n';
}
