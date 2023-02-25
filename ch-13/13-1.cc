#include <cstdio>
#include <array>
#include <vector>

int main()
{
    /* build array with first 20 # in fibonacci sequence */
    std::array<unsigned long, 20> fib{1, 1};
    for(auto iter = fib.begin()+2; iter < fib.end(); iter++)
        *iter = *(iter-1) + *(iter-2);

    /* default construct vector of unsigned longs */
    std::vector<unsigned long> vec;

    /* print capacity*/
    printf("vec capacity = %ld\n", vec.capacity());

    /* reserve 10 unsigned longs */
    vec.reserve(10);

    /* insert the elements from fib */
    vec.insert(vec.begin(), std::begin(fib), std::end(fib));

    /* vector had to expand the capacity beyond 10 to fit all 20 elements */
    printf("vec capacity = %ld\n", vec.capacity());

    /* print it */
    for(auto el : vec)
        printf("%ld ", el);
    printf("\n");

    return 0;
}