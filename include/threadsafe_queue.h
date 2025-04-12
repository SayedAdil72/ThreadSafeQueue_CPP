#pragma once

#include <condition_variable>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>

/**
 * @brief A thread-safe, bounded queue using lock-free push and blocking pop.
 *
 * Designed for single producer and single consumer (SPSC) communication.
 * Automatically drops the oldest element if full.
 *
 * @tparam T Type of elements stored in the queue.
 */
template<typename T>
class Queue 
{
private:
    T* buffer;                             // Raw pointer to dynamically allocated element storage.
    const size_t capacity;                 // Maximum size of the queue.
    std::atomic<size_t> head;              // Index of the next element to pop.
    std::atomic<size_t> tail;              // Index of the next element to write.
    std::atomic<size_t> count;             // Number of elements currently in the queue.

    mutable std::mutex pop_mutex;          // Mutex used for blocking/timeout pop operations.
    std::condition_variable cond_var;      // Condition variable used for blocking/timeout pop operations.

public:
    /**
     * @brief Constructs a queue with a fixed maximum size.
     * @param size Maximum number of elements.
     * @throws std::invalid_argument if size <= 0.
     */
    explicit Queue(int size) : capacity(size), head(0), tail(0), count(0) 
    {
        if (size <= 0)
            throw std::invalid_argument("Queue size must be positive");

        buffer = new T[capacity];
    }

    /**
     * @brief Destructor.
     */
    ~Queue() 
    {
        delete[] buffer;
    }

    // Disable copy and move operations to prevent unsafe pointer ownership.
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    Queue(Queue&&) = delete;
    Queue& operator=(Queue&&) = delete;

    /**
     * @brief Push an element to the queue.
     * If full, the oldest element is dropped.
     * Non-blocking, lock-free.
     *
     * @param element Element to insert.
     */
    void Push(const T& element)
    {
        size_t local_tail = tail.load(std::memory_order_relaxed);
        size_t local_head = head.load(std::memory_order_acquire);
        size_t current_count = count.load(std::memory_order_relaxed);

        if (current_count == capacity) 
        {
            // Drop the oldest element (advance head)
            head.store((local_head + 1) % capacity, std::memory_order_release);
            count.fetch_sub(1, std::memory_order_relaxed);
        }

        // Insert new element
        buffer[local_tail] = element;
        tail.store((local_tail + 1) % capacity, std::memory_order_release);
        count.fetch_add(1, std::memory_order_relaxed);

        cond_var.notify_one();
    }

    /**
     * @brief Pops the next element.
     * Blocks indefinitely if the queue is empty.
     *
     * @return The popped element.
     */
    T Pop()
    {
        std::unique_lock<std::mutex> lock(pop_mutex);
        cond_var.wait(lock, [this] {
            return count.load(std::memory_order_acquire) > 0;
            });

        size_t local_head = head.load(std::memory_order_relaxed);
        T item = buffer[local_head];
        head.store((local_head + 1) % capacity, std::memory_order_release);
        count.fetch_sub(1, std::memory_order_relaxed);
        return item;
    }

    /**
     * @brief Pops the next element with a timeout.
     *
     * @param milliseconds Timeout in milliseconds.
     * @return The popped element.
     * @throws std::runtime_error if timeout expires.
     */
    T PopWithTimeout(int milliseconds) 
    {
        std::unique_lock<std::mutex> lock(pop_mutex);
        if (!cond_var.wait_for(lock, std::chrono::milliseconds(milliseconds), [this] {
            return count.load(std::memory_order_acquire) > 0;
            })) 
        {
            throw std::runtime_error("Timeout while waiting for element");
        }

        size_t local_head = head.load(std::memory_order_relaxed);
        T item = buffer[local_head];
        head.store((local_head + 1) % capacity, std::memory_order_release);
        count.fetch_sub(1, std::memory_order_relaxed);
        return item;
    }

    /**
     * @brief Gets the current number of elements in the queue.
     * @return Count of stored elements.
     */
    int Count() const 
    {
        return static_cast<int>(count.load(std::memory_order_acquire));
    }

    /**
     * @brief Gets the maximum capacity of the queue.
     * @return Max number of elements that can be stored.
     */
    int Size() const 
    {
        return static_cast<int>(capacity);
    }
};
