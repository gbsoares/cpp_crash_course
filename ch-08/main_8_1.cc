#include <cstdio>

#include "FibonacciRange.hh"

int main (int argc, char** argv)
{
    for (const auto i : FibonacciRange{ 5000 })
    {
        printf("%d ", i);
    }
    return 0;
}