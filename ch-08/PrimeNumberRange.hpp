#ifndef __PRIMENUMBERRANGE_H__
#define __PRIMENUMBERRANGE_H__

struct PrimeNumberIterator
{
    bool operator!=(int x) const;
    PrimeNumberIterator& operator++();
    int operator*() const;

private:
    bool is_prime(int x) const;
    int prime{ 2 };
};

struct PrimeNumberRange
{
    explicit PrimeNumberRange(int max);    
    PrimeNumberIterator begin() const;
    int end() const;

private:
    const int max;
};

#endif /* #ifndef __PRIMENUMBERRANGE_H__*/