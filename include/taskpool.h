#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class Task
{
public:
    virtual void operator()() = 0;
};

class TaskPool
{
public:
    typedef std::function<void()> CompletionFunc;

    TaskPool(const std::vector<std::shared_ptr<Task>>& queue, CompletionFunc completion, bool multithread = true);
    TaskPool() = delete;
    ~TaskPool();

    void WaitToComplete();

private:
    std::vector<std::thread> _threads;
    std::vector<std::shared_ptr<Task>> _queue;
    std::mutex _mutex;
    CompletionFunc _completion;
};

inline TaskPool::TaskPool(const std::vector<std::shared_ptr<Task>>& queue, CompletionFunc completion, bool multithread)
    : _queue(queue)
    , _completion(completion)
{
    const size_t coreCount = std::thread::hardware_concurrency();
    size_t threadCount = multithread ? std::min(coreCount, queue.size()) : 1;

    for (size_t i = 0; i < threadCount; ++i) {
        // Create a thread with a lambda that loops through the queue
        // of file paths and processes them.
        _threads.emplace_back([this]{
            while (true) 
            {
                std::shared_ptr<Task> task;

                {
                    std::unique_lock<std::mutex> lock(_mutex);

                    if (_queue.empty())
                    {
                        break;
                    }
                    task = _queue.back();
                    _queue.pop_back();
                }

                (*task)();
            }
        });
    }
}

inline TaskPool::~TaskPool()
{
    WaitToComplete();
}

inline void TaskPool::WaitToComplete()
{
    for (auto &thread : _threads) {
        thread.join();
    }
    _threads.clear();

    if (_completion)
    {
        _completion();
        _completion = nullptr;
    }
}
