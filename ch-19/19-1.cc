#include <iostream>
#include <mutex>
#include <future>
#include <atomic>

struct SpinLock
{
    SpinLock() = default;
    ~SpinLock() = default;
    SpinLock(const SpinLock &) = delete;
    void lock() { while(the_lock.test_and_set()); }
    bool try_lock() { return !the_lock.test_and_set(); }
    void unlock() { return the_lock.clear(); }
private:
    std::atomic_flag the_lock{ ATOMIC_FLAG_INIT };
};

void goat_rodeo()
{
    const size_t iterations{1'000'000};
    int tin_cans_available{};
    SpinLock spin_lock;
    auto eat_cans = std::async(std::launch::async, [&] {
        for(size_t i{}; i < iterations; i++) 
        {
            std::lock_guard<SpinLock> guard{ spin_lock };
            tin_cans_available--;
        } 
    });
    auto deposit_cans = std::async(std::launch::async, [&] {
        for(size_t i{}; i < iterations; i++) 
        {
            std::lock_guard<SpinLock> guard{ spin_lock };
            tin_cans_available++;
        }
    });
    eat_cans.get();
    deposit_cans.get();
    std::cout << "Tin cans: " << tin_cans_available << "\n";
}

int main()
{
    goat_rodeo();
    goat_rodeo();
    goat_rodeo();
    return 0;
}