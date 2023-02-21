#ifndef __FIBONACCIITERATOR_H__
#define __FIBONACCIITERATOR_H__

struct FibonacciIterator
{
    bool operator!=(int x) const;
    FibonacciIterator& operator++();
    int operator*() const;

private:
    int current{ 1 };
    int last{ 1 };
};

#endif /* #ifndef __FIBONACCIITERATOR_H__*/