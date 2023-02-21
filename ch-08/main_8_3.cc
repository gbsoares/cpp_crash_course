#include <cstdio>

#include "FibonacciRange.hh"
#include "PrimeNumberRange.hh"

int main (int argc, char** argv)
{
    printf("Fibonacci: ");
    for (const auto i : FibonacciRange{ 5000 })
    {
        printf("%d ", i);
    }
    printf("\n");
    printf("Prime: ");
    for (const auto i : PrimeNumberRange{ 5000 })
    {
        printf("%d ", i);
    }
     printf("\n");
    return 0;
}