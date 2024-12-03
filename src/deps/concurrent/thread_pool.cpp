#include "thread_pool.h"

namespace concurrent {

ThreadPool::ThreadPool(size_t num_threads) : m_stop(false) {
    LOG_DEBUG("ThreadPool constructor, num_threads: %zu", num_threads);
    for (size_t i = 0; i < num_threads; ++i) {
        m_workers.emplace_back(std::thread(&ThreadPool::workThread, this));
    }
}

ThreadPool::~ThreadPool() {
    m_stop = true;
    m_tasks.clear();
    for (std::thread& worker : m_workers) {
        worker.join();
    }
}

void ThreadPool::workThread() {
    while (!m_stop) {
        std::function<void()> task;
        {
            m_tasks.wait([this] { return m_stop || !m_tasks.empty(); });

            task = m_tasks.front();
            m_tasks.pop();
        }
        task();
    }

}

void ThreadPool::addTask(std::function<void()> task) {
    m_tasks.push(task);
}

}
