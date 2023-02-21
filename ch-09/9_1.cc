#include <cstdio>

template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial)
{
    Out res{ initial };

    for(size_t i{}; i < length; i++)
    {
        res = function(res, input[i]);
    }

    return res;
}

int main()
{
    int data[]{ 100, 200, 300, 400, 500 };
    size_t data_len = sizeof(data)/sizeof(data[0]);
    auto sum = fold([](auto x, auto y){ return x + y; }, data, data_len, 0);
    printf("Sum: %d\n", sum);
    auto max = fold([](auto x, auto y){ return x > y ? x : y; }, data, data_len, data[0]);
    printf("Max: %d\n", max);
    auto min = fold([](auto x, auto y){ return x < y ? x : y; }, data, data_len, data[0]);
    printf("Min: %d\n", min);
    auto gt200 = fold([](auto x, auto y){ return y > 200 ? ++x : x; }, data, data_len, 0);
    printf("# > 200: %d\n", gt200);
    return 0 ;
}


