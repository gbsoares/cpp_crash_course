#include "PrimeNumberRange.hh"

PrimeNumberRange::PrimeNumberRange(int max) : 
    max { max } 
{
}
    
PrimeNumberIterator PrimeNumberRange::begin() const
{
    return PrimeNumberIterator{};
}

int PrimeNumberRange::end() const
{
    return max;
}

bool PrimeNumberIterator::operator!=(int x) const
{
    return x >= prime;
}

bool PrimeNumberIterator::is_prime(int x) const
{
    if(x % 2 == 0 || x % 3 == 0)
        return false;

    for (int i = 5; i * i <= x; i = i + 6)
    {
        if (x % i == 0 || x % (i + 2) == 0)
            return false;
    }

    return true;
}

PrimeNumberIterator& PrimeNumberIterator::operator++()
{
    while(!is_prime(++prime));
        
    return *this;
}

int PrimeNumberIterator::operator*() const
{
    return prime;
}
