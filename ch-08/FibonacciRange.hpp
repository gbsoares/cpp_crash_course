// #ifndef __FIBONACCIRANGE_H__
// #define __FIBONACCIRANGE_H__

#include "FibonacciIterator.hpp"

struct FibonacciRange
{
    explicit FibonacciRange(int max);    
    FibonacciIterator begin() const;
    int end() const;

private:
    const int max;
};


// #endif /* #ifndef __FIBONACCIRANGE_H__*/