#include <vector>
#include <cstdio>

template <typename T>
int sum(T begin, T end)
{
    int ret{};
    for(T i = begin; i < end; i++)
    {
        ret += *i;
    }
    return ret;
}

int main()
{
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7};
    auto s = sum(vec.begin(), vec.end());
    printf("Sum = %d\n", s);

    return 0;
}