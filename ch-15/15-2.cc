#include <cstdio>
#include <string>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("must pass exactly 1 argument");
        return -1;
    }

    std::string str{argv[1]};
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