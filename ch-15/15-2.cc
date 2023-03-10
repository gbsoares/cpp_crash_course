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

    std::string res{"is"};
    for(auto itr1 = str.begin(), itr2 = str.end()-1; itr1 < itr2; itr1++, itr2--)
    {
        if(*itr1 != *itr2)
        {
            res.append(" not");
            break;
        }
    }

    printf("'%s' %s a palindrome\n", str.c_str(), res.c_str());
    return 0;
}