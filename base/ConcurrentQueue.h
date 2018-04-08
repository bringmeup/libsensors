#ifndef CONCURRENTQUEUE_H_
#define CONCURRENTQUEUE_H_

#include <thread>
#include <queue>

template<typename Data>
class ConcurrentQueue
{
private:
    std::queue<Data> mQueue;
    mutable std::mutex mMutex;
    std::condition_variable mConditionVariable;
public:
    void push(Data const& data)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(data);
        lock.unlock();
        mConditionVariable.notify_one();
    }

    bool empty() const
    {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

    bool tryPop(Data& popped_value)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if(mQueue.empty())
        {
            return false;
        }

        popped_value=mQueue.front();
        mQueue.pop();
        return true;
    }

    void pop(Data& popped_value)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        while(mQueue.empty())
        {
            mConditionVariable.wait(lock);
        }

        popped_value=mQueue.front();
        mQueue.pop();
    }

};

#endif // CONCURRENTQUEUE_H_
