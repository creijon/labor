#include <taskpool.h>

TaskPool::TaskPool(const std::vector<std::string>& filePaths, TaskFunc task, CompletionFunc completion, bool multithread)
    : _filePaths(filePaths)
    , _task(task)
    , _completion(completion)
{
    const size_t coreCount = std::thread::hardware_concurrency();
    size_t threadCount = multithread ? std::min(coreCount, filePaths.size()) : 1;

    for (size_t i = 0; i < threadCount; ++i) {
        // Create a thread with a lambda that loops through the queue
        // of file paths and processes them.
        _threads.emplace_back([this]{
            while (true) 
            {
                bool finalTask = false;
                std::string filePath;
                {
                    std::unique_lock<std::mutex> lock(_mutex);
                    if (_filePaths.empty())
                    {
                        break;
                    }
                    filePath = _filePaths.back();
                    _filePaths.pop_back();

                    finalTask = _filePaths.empty();
                }

                _task(filePath);

                if (_completion && finalTask)
                {
                    _completion();
                }
            }
        });
    }
}

TaskPool::~TaskPool()
{
    WaitToComplete();
}

void TaskPool::WaitToComplete()
{
    for (auto &thread : _threads) {
        thread.join();
    }
    _threads.clear();
}
