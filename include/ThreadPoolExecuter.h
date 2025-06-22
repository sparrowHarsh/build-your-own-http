#pragma once
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

/**
 * @brief Enqueues a task into the thread pool and returns a future to retrieve the result.
 * @tparam F Callable type.
 * @tparam Args Argument types.
 * @param f Callable to execute.
 * @param args Arguments to pass to the callable.
 * @return std::future<typename std::invoke_result_t<F, Args...>> Future to retrieve the result.
 */
template<class F, class... Args>
auto ThreadPoolExecuter::enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>> {
    using returnType = typename std::invoke_result_t<F, Args...>;

    auto taskPtr = std::make_shared<std::packaged_task<returnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<returnType> result = taskPtr->get_future();

    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (stop) {
            throw std::runtime_error("Cannot submit tasks to a stopped ThreadPoolExecuter");
        }
        task.emplace([taskPtr]() { (*taskPtr)(); });
    }

    queueCondition.notify_one();
    return result;
}

