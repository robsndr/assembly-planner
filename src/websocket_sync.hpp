#pragma once

#include <mutex>
#include <condition_variable>

namespace websocket_sync{
        
    class Semaphore {
        public:
            Semaphore (int threshold_ = 2)
            : count(0),
              threshold(threshold_)
            {
            }

            inline void reset(int threshold_){
                count = 0;
                threshold = threshold_;
            }
            
            inline void notify() {
                std::unique_lock<std::mutex> lock(mtx);
                count++;
                std::cout << "thread " <<  " notify" << std::endl;
                //notify the waiting thread
                cv.notify_one();
            }

            inline void wait() {
                std::unique_lock<std::mutex> lock(mtx);
                while(count < threshold) {
                    cv.wait(lock);
                }
                count = 0;
            }

        private:
            std::mutex mtx;
            std::condition_variable cv;
            int count;
            int threshold;

    };

    Semaphore semaphore;
}