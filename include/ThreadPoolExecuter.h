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
 * @brief Constructs a ThreadPoolExecuter with the given number of threads.
 * @param numOfThread Number of worker threads to create.
 */
inline ThreadPoolExecuter :: ThreadPoolExecuter(int numOfThread) : stop(false){
    for(int i = 0; i < numOfThread; i++){
        workers.emplace_back([this]{
            // Create and add a new worker thread to the workers vector.
            // Each worker thread runs a lambda function that loops forever, waiting for and executing tasks.
            while(true){

                // Each worker thread runs an infinite loop, repeatedly fetching and executing tasks.
                std::function<void()> task;

                // Begin critical section for accessing the task queue.
                {
                    // Acquire a lock on the queue mutex to ensure thread-safe access to the task queue.
                    std::unique_lock<std::mutex> lock(this->queueMutex);

                    // Wait until either stop is true or there is at least one task in the queue.
                    this->queueCondition.wait(lock,[this]{
                        return this->stop || !this->task.empty();
                    });

                    // If stop is true and the task queue is empty, exit the thread.
                    if(this->stop && this->task.empty()){
                        return;
                    }

                    // Retrieve the next task from the queue and remove it.
                    task = std::move(this->task.front());
                    this->task.pop();
                }
            }
        });
    }
}

/**
 * @brief Shuts down the thread pool and joins all worker threads.
 */
inline void ThreadPoolExecuter :: shutdown(){
    {
        // Lock the queue mutex and set the stop flag to true to signal all threads to stop.
        std::unique_lock<std::mutex> lock(this->queueMutex);
        stop = true;
    }

    // Notify all worker threads to check the stop condition again.
    queueCondition.notify_all();
    
    // Join all worker threads to free their resources.
    for(std::thread& worker:workers){
        if(worker.joinable()){
            worker.join();
        }
    }
}

/**
 * @brief Destructor for ThreadPoolExecuter. Shuts down the thread pool.
 */
inline ThreadPoolExecuter :: ~ThreadPoolExecuter(){
    shutdown();
}

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


