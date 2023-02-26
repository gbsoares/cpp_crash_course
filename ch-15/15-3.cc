#include <cstdio>
#include <string>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("must pass at least 1 argument\n");
        return -1;
    }

    /* create a single string from all the passed parameters */
    std::string str{};
    for(int index{1}; index < argc; index++)
    {
        str += argv[index];
        str += (index == argc - 1 ? "" : " "); 
    }

    size_t count{};
    /* count the number of vowels in str */
    for(auto c : str)
    {
        if( c == 'a' || c == 'A' || 
            c == 'e' || c == 'E' ||
            c == 'i' || c == 'I' ||
            c == 'o' || c == 'O' ||
            c == 'u' || c == 'U' )
        {
            count++;
        }
    }

    printf("%ld vowels in '%s'\n", count, str.c_str());
    return 0;
}