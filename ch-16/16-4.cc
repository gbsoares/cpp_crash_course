#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <map>

/* custom output stream operator to print character histograms */
std::ostream& operator<<(std::ostream& os, std::map<char, size_t> map)
{
    for(const auto& c : map)
    {
        os << c.first << ": ";
        for(size_t i{}; i < c.second; i++, os << "*");
        os << "\n";
    }
    return os;
}

int main(int argc, char** argv)
{
    /* program takes 1 argument equal to file to open */
    if(argc != 2)
        exit(0);
    
    /* open input file */
    std::ifstream f{ argv[1], std::ios_base::in };
    if(!f.is_open())
        exit(0);
    
    size_t wc{};
    size_t running_word_len{};
    std::map<char, size_t> hist{};

    /* read file word by word and collect stats */
    std::string w;
    while(f >> w)
    {
        wc++;
        running_word_len += w.size();
        for(const auto& c : w)
        {
            if(c >= 'a' && c <= 'z')
                hist[c - 'a' + 'A']++;
            else
                hist[c]++;
        }
    }

    /* print stats */
    std::cout << "Word Count: " << wc << std::endl;
    std::cout << "Avg Word Length: " << static_cast<double>(running_word_len) / static_cast<double>(wc) << std::endl;
    std::cout << "Character Histogram:\n" << hist;

    return 0;
}