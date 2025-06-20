class ThreadPoolExecuter {
    // Need
    // enque -> jopbs
    // mutex for those enque
    // vector of threads (threads container)


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
};

ThreadPoolExecuter :: ThreadPoolExecuter(int numOfThread) : stop(false){
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

void ThreadPoolExecuter :: shutdown(){
    {
        // Lock the queue mutex and set the stop flag to true to signal all threads to stop.
        std::unique_lock<std::mutex> lock(this->queueMutex);
        stop = true;
    }

    // Notify all worker threads to check the stop condition again.
    queueConditon.notify_all();
    
    // Join all worker threads to free their resources.
    for(std::thread& worker:workers){
        if(worker.joinable()){
            worker.join();
        }
    }
}

ThreadPoolExecuter :: ~ThreadPoolExecuter(){
    shutdown();
}


