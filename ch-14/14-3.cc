#include <chrono>
#include <forward_list>
#include <vector>
#include <cstdio>

struct Stopwatch
{
    Stopwatch(std::chrono::nanoseconds &result) : result{result},
                                                  start{std::chrono::system_clock::now()} {}

    ~Stopwatch()
    {
        result = std::chrono::system_clock::now() - start;
    }

private:
    std::chrono::nanoseconds &result;
    const std::chrono::time_point<std::chrono::system_clock> start;
};

int main()
{
    std::chrono::nanoseconds elapsed;
    {
        std::forward_list<int> fwd_list(10'000'000);
        Stopwatch stopwatch(elapsed);
        auto itr = fwd_list.begin();
        std::advance(itr, 10'000'000);
    }
    printf("Forward list - Elapsed: %g s.\n", elapsed.count() / 1'000'000'000.);

    elapsed = std::chrono::nanoseconds::zero();
    {
        std::vector<int> vec(10'000'000);
        Stopwatch stopwatch(elapsed);
        auto itr = vec.begin();
        std::advance(itr, 10'000'000);
    }
    printf("Vector - Elapsed: %g s.\n", elapsed.count() / 1'000'000'000.);
    return 0;
}