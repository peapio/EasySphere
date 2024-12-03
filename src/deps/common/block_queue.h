#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace common {

template <typename T>
class BlockQueue {
public:
    BlockQueue(size_t capacity = 1000);
    ~BlockQueue();

    bool push(const T& item);
    bool pop(T& item);
    bool empty() const;
    bool full() const;
    T front() const;
    T back() const;
    size_t size() const;
    size_t capacity() const;
    void clear();
    void close();
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    size_t m_capacity;
    bool m_bClose;
};

template <typename T>
BlockQueue<T>::BlockQueue(size_t capacity) : m_capacity(capacity), m_bClose(false) {
    assert(capacity > 0);
}

template <typename T>
BlockQueue<T>::~BlockQueue() {
    close();
}

template <typename T>
bool BlockQueue<T>::push(const T& item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] { return m_queue.size() < m_capacity || m_bClose; });
    if (m_bClose) {
        return false;
    }
    m_queue.push(item);
    m_condition.notify_one();
    return true;
}

template <typename T>
bool BlockQueue<T>::pop(T& item) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this] { return !m_queue.empty() || m_bClose; });
    if (m_bClose) {
        return false;
    }
    item = m_queue.front();
    m_queue.pop();
    m_condition.notify_one();
    return true;
}

template <typename T>
bool BlockQueue<T>::empty() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

template <typename T>
bool BlockQueue<T>::full() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size() >= m_capacity;
}

template <typename T>
T BlockQueue<T>::front() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.front();
}

template <typename T>
T BlockQueue<T>::back() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.back();
}

template <typename T>
size_t BlockQueue<T>::size() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
}

template <typename T>
size_t BlockQueue<T>::capacity() const {
    return m_capacity;
}

template <typename T>
void BlockQueue<T>::clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.clear();
    m_condition.notify_all();
}

template <typename T>
void BlockQueue<T>::close() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_bClose = true;
    m_condition.notify_all();
}

}