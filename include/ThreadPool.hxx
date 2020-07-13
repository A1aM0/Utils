//
// Created by y on 2020/7/13.
//

#ifndef THREADPOOL_HXX
#define THREADPOOL_HXX

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>
#include <queue>
#include <list>
#include <functional>

namespace multiThread {
    template <unsigned ThreadCount = 10>
    class ThreadPool {

        std::array<std::thread, ThreadCount> threads;
        std::list<std::function<void(void)>> jobs;

        std::atomic_int         jobs_left;
        std::atomic_bool        bailout;
        std::atomic_bool        finished;
        std::condition_variable jobs_waiting_cv;
        std::condition_variable wait_cv;
        std::mutex              wait_mutex;
        std::mutex              jobsList_mutex;


        void Task() {
            while( !bailout ) {
                next_job()();
                --jobs_left;
                wait_cv.notify_one();
            }
        }


        std::function<void(void)> next_job() {
            std::function<void(void)> res;
            std::unique_lock<std::mutex> job_lock(jobsList_mutex );


            jobs_waiting_cv.wait(job_lock, [this]() ->bool { return jobs.size() || bailout; } );


            if( !bailout ) {
                res = jobs.front();
                jobs.pop_front();
            }
            else {
                res = []{};
                ++jobs_left;
            }
            return res;
        }

    public:
        ThreadPool()
                : jobs_left( 0 )
                , bailout( false )
                , finished( false )
        {
            for( unsigned i = 0; i < ThreadCount; ++i )
                threads[ i ] = std::thread( [this]{ this->Task(); } );
        }


        ~ThreadPool() {
            JoinAll();
        }


        inline unsigned Size() const {
            return ThreadCount;
        }


        inline unsigned JobsRemaining() {
            std::lock_guard<std::mutex> guard(jobsList_mutex );
            return jobs.size();
        }


        void AddJob( std::function<void(void)> job ) {
            std::lock_guard<std::mutex> guard(jobsList_mutex );
            jobs.emplace_back(job );
            ++jobs_left;
            jobs_waiting_cv.notify_one();
        }


        void JoinAll( bool WaitForAll = true ) {
            if( !finished ) {
                if( WaitForAll ) {
                    WaitAll();
                }

                bailout = true;
                jobs_waiting_cv.notify_all();

                for( auto &x : threads )
                    if( x.joinable() )
                        x.join();
                finished = true;
            }
        }


        void WaitAll() {
            if( jobs_left > 0 ) {
                std::unique_lock<std::mutex> lk( wait_mutex );
                wait_cv.wait(lk, [this]{ return this->jobs_left == 0; } );
                lk.unlock();
            }
        }
    };
}

template<class T>
class Singleton {
    Singleton(Singleton const&);
    Singleton& operator= (Singleton const&);
    static T* instance;

protected:
    Singleton();
    ~Singleton();

public:
    template<typename ... Args> static T& getInstance(Args ... args) {
        if (!instance) instance = new T(std::forward<Args>(args)...);
        return *instance;
    }
};

template<class T> T* Singleton<T>::instance = nullptr;

typedef Singleton<multiThread::ThreadPool<300>> ThreadPoolSingleton ;

#endif //UTILSTEST_THREADPOOL_HXX
