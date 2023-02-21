#include <cstdio>
#include <cstring>
#include <map>

int main(int argc, char* argv[])
{
    std::map<size_t, int> len_map{};
    for(int x{1}; x < argc; x++)
    {
        char* input = argv[x];
        size_t input_len = strlen(input);

        printf("Input string (%ld): '%s'\n", input_len, input);
        len_map[input_len]++;
    }

    for(auto el : len_map)
    {
        printf("%3ld: ", el.first);
        int x = el.second;
        while(x--) printf("*");
        printf("\n");
    }

    return 0;
}