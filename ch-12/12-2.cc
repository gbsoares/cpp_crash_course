#include <cstdio>
#include <random>

const char alphabet[] = {'a', 'b', 'c', 'd', '*', '1', '6', '_', 'X'};

int main()
{
    int num_pwds = 10;
    int pwd_length = 10;

    uint_fast64_t prn_seed{999222};
    std::mt19937_64 mt_engine{prn_seed};
    std::uniform_int_distribution<int> int_d{0 , sizeof(alphabet)/sizeof(alphabet[0])-1};

    for(size_t i{1}; i <= num_pwds; i++)
    {
        char pwd[pwd_length + 1]{};
        for(size_t j{}; j < pwd_length; j++)
        {
            pwd[j] = alphabet[int_d(mt_engine)];
        }

        printf("%s\n", pwd);
    }

    return 0;
}