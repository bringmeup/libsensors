/*
 * util.h
 *
 *  Created on: Apr 9, 2018
 *      Author: nazgee
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <semaphore.h>
#include <mutex>

#define SENSOR_DEBUG

#ifdef SENSOR_DEBUG
#define DEBUG(format, ...) ALOGD("%s " format, __FUNCTION__, ## __VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

class SafeThread {
    typedef std::function<void()> worker_t;
    worker_t mWorkFunc;
    std::thread mThread;
    std::atomic<bool> mThreadQuit;

    bool mPause=true;
    std::condition_variable mCV;
    std::mutex mMutex;



public:
    SafeThread(const worker_t& work_func) :
        mWorkFunc(work_func), mThread(&SafeThread::worker, this), mThreadQuit(false) {
    }

    void pause(){
         std::lock_guard<std::mutex> lk(mMutex);
         mPause=true;
    }
    void resume(){
         std::lock_guard<std::mutex> lk(mMutex);
         mPause=false;
         mCV.notify_one();
    }

    void requestExit() {
        std::lock_guard<std::mutex> lk(mMutex);
        mThreadQuit = true;
        mPause=false;
        mCV.notify_one();
    }

    ~SafeThread() {
        requestExit();
        if (mThread.joinable()) {
            mThread.join();
        }
    }

private:

    void worker()
    {
        while (!mThreadQuit)
        {
            while(mPause){
               std::unique_lock<std::mutex> lk(mMutex);
               mCV.wait(lk);
               lk.unlock();
            }

            if (!mThreadQuit) {
                mWorkFunc();
            }
        }
    }
};

#endif /* UTIL_H_ */
