#include <cstdio>
#include <cstring>
#include <set>

int main(int argc, char **argv)
{
    /* create set with custom comparator */
    auto cmp = [](const char* x, const char* y) { return strcmp(x, y) < 0 ? true : false; };
    std::set<const char*, decltype(cmp)> args{cmp};

    /* insert argument list parameters */
    for(int i = 0; i < argc; i++)
    {
        args.insert(argv[i]);
    }

    /* print the set */
    for(auto el : args)
    {
        printf("%s\n", el);
    }

    return 0;
}