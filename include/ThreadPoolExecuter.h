#include <vector>
#include <iostream>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <stdexcept>

class ThreadPoolExecuter {
    private:
        std::vector<std::thread> workers;   // worker thread 
        std::queue<std::function<void()>> task;  // a lemda function of task in queue
        std::mutex queueMutex;
        std::condition_variable queueCondition;
        bool stop;


    public:
        ThreadPoolExecuter(int numOfThread);
        ~ThreadPoolExecuter();
        void shutdown();

        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>>;
};
