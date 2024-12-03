#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>
#include <vector>
#include <future>
#include "common/log.h"
#include "common/single.h"
#include "common/block_queue.h"

namespace concurrent {

class ThreadPool {
public:
    ThreadPool(size_t num_threads = 8);
    ~ThreadPool();

    void addTask(std::function<void()> task);



private:
    void workThread();

    std::vector<std::thread> m_workers;
    common::BlockQueue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_condition;

    bool m_stop;
};

}
