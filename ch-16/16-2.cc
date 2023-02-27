#include <iostream>

int main()
{
    std::cout << "Program echos in ALL_CAPS:" << std::endl;
    std::string word;
    while(std::getline(std::cin, word))
    {
        for(size_t i{}; i < word.size(); i++)
        {
            if(word[i] >= 'a' && word[i] <= 'z')
                word[i] = word[i] - 'a' + 'A'; 
        }
        std::cout << word << "\n";
    }

    return 0;
}