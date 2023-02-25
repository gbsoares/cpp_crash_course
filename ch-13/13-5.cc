#include <cstdio>
#include <chrono>
#include <random>
#include <map>

unsigned long fib_sum(size_t n)
{
    /* construct a vector with the first n fibonacci numbers */
    std::vector<unsigned long> fib{1, 1};
    for(size_t i{2}; i < n; i++)
        fib.push_back(fib[i-1]+fib[i-2]);

    unsigned long sum{};
    for(auto el : fib)
    {
        sum += el;
    }
    return sum;
}

unsigned long cached_fib_sum(const size_t& n)
{
    static std::map<long, long> cache;
    auto x = cache.find(n);
    if(x == cache.end())
    {
        auto [iter, is_new] = cache.insert({n, fib_sum(n)});
        x = iter;
    }
    return x->second;
}

long random(const long lower_bound, const long upper_bound)
{
    static std::mt19937_64 mt_engine{ 102787 };
    static std::uniform_int_distribution<long> int_d{lower_bound, upper_bound};
    return int_d(mt_engine);
}

struct Stopwatch
{
    Stopwatch(std::chrono::nanoseconds& result) : 
        result  { result },
        start   { std::chrono::system_clock::now() }{ }

    ~Stopwatch()
    {
        result = std::chrono::system_clock::now() - start;
    }

private:
    std::chrono::nanoseconds& result;
    const std::chrono::time_point<std::chrono::system_clock> start;
};

int main(int argc, char **argv)
{
    std::chrono::nanoseconds elapsed_simple;
    {
        size_t samples {1'000'000};
        Stopwatch stopwatch(elapsed_simple);
        volatile double answer;
        while(samples--)
        {
            answer = fib_sum(random(100, 200));
        }
    }
    printf("Simple - Elapsed: %g s.\n", elapsed_simple.count() / 1'000'000'000.);
    
    std::chrono::nanoseconds elapsed_cached;
    {
        size_t samples {1'000'000};
        Stopwatch stopwatch(elapsed_cached);
        volatile double answer;
        while(samples--)
        {
            answer = cached_fib_sum(random(100, 200));
        }
    }
    printf("Cached - Elapsed: %g s.\n", elapsed_cached.count() / 1'000'000'000.);

    return 0;
}