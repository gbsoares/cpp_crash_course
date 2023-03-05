#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

template<typename T>
struct Q
{
    Q(){};
    bool empty() const  { return std::lock_guard<std::mutex>(q_mutex), q.empty(); }
    size_t size() const { return std::lock_guard<std::mutex>(q_mutex), q.size(); }
    T front() const     { return std::lock_guard<std::mutex>(q_mutex), q.front(); }
    T back() const      { return std::lock_guard<std::mutex>(q_mutex), q.back(); }
    void push( T item ) { return std::lock_guard<std::mutex>(q_mutex), q.push(item), q_cv.notify_all(); }
    T pop ()            { return std::lock_guard<std::mutex>(q_mutex), q.pop(); }
    
    T wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(q_mutex);
        q_cv.wait(lock, [&]{ return !q.empty();});
        T ret = q.back();
        q.pop();
        lock.unlock();
        return ret;
    }
private:
    std::queue<T> q{};
    mutable std::mutex q_mutex;
    std::condition_variable q_cv{};
};

int main()
{
    using namespace std::literals::chrono_literals;

    Q<int> q{};
    auto ftr = std::async(std::launch::async, [&] { return q.wait_and_pop(); });

    const auto waiting1 = ftr.wait_for(1ms);
    std::cout << "Wait 1ms and checking if the async task has popped a value..." << std::endl;
    std::cout << "- future_status::timeout = " << std::boolalpha << static_cast<bool>(waiting1 == std::future_status::timeout) << std::endl;
    std::cout << "- future_status::ready = " << std::boolalpha << static_cast<bool>(waiting1 == std::future_status::ready) << std::endl;

    std::cout << "\nPushing a value\n" << std::endl;
    q.push(1);

    const auto waiting2 = ftr.wait_for(1ms);
    std::cout << "Wait 1ms and checking if the async task has popped a value..." << std::endl;
    std::cout << "- future_status::timeout = " << std::boolalpha << static_cast<bool>(waiting2 == std::future_status::timeout) << std::endl;
    std::cout << "- future_status::ready = " << std::boolalpha << static_cast<bool>(waiting2 == std::future_status::ready) << std::endl;

    std::cout << "\n# elements left: " << q.size() << std::endl;
    return 0;
}