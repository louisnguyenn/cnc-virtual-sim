#pragma once
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T> class CommandQueue
{
  private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_done{false};

  public:
    /// @brief push value onto queue (called by parser thread)
    /// @param value
    void push(T value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(value));
        m_cv.notify_one(); // notify consumer thread
    }

    /// @brief pop a value off the queue (called by simulator thread)
    ///        blocks and waits if queue is empty
    /// @return optional
    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cv.wait(lock, [this] {
            // return if something is in the queue or done
            return !m_queue.empty() || m_done;
        });

        // if queue is empty return nothing
        if (m_queue.empty())
        {
            return std::nullopt;
        }

        // take the first element in the queue
        T value = std::move(m_queue.front());
        m_queue.pop(); // pop it

        return value; // return it
    }

    /// @brief called when parser is finished
    //         tells consumer thread it can stop waiting
    void done()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_done = true;
        m_cv.notify_all();
    }

    /// @brief check if queue is empty
    /// @return bool (true is empty, false if not)
    bool isEmpty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        return m_queue.empty();
    }
};
