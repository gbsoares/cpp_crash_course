#include <cstdio>

template <typename Fn, typename In>
constexpr bool all(Fn function, In* input, size_t length)
{
    for(int i{}; i < length; i++)
    {
        if(!function(input[i]))
            return false;
    }
    return true;
}

void test(bool x)
{
    if(x)
        printf("All elements greater than 100.\n");
    else
        printf("Not all elements are greater than 100.\n");
}

int main()
{
    int data[]{ 100, 200, 300, 400, 500 };
    size_t data_len = 5;
    auto all_gt100 = all([](auto x){ return x > 100; }, data, data_len);
    test(all_gt100);
    data[0] = 200;
    all_gt100 = all([](auto x){ return x > 100; }, data, data_len);
    test(all_gt100);
    return 0;
}
