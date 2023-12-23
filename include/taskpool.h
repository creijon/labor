#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <functional>
#include <string>

class TaskPool
{
public:
    typedef std::function<bool(const std::string&)> TaskFunc;
    typedef std::function<void()> CompletionFunc;

    TaskPool(const std::vector<std::string>& filePaths, TaskFunc task, CompletionFunc completion, bool multithread = true);
    TaskPool() = delete;
    ~TaskPool();

    void WaitToComplete();

private:
    std::vector<std::thread> _threads;
    std::vector<std::string> _filePaths;
    std::mutex _mutex;
    TaskFunc _task;
    CompletionFunc _completion;
};
