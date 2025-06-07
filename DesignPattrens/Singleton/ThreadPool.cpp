#include<iostream>
#include <mutex>
#include <vector>
#include <thread>
using namespace std;

class ThreadPool 
{
    private:
        static ThreadPool* instance;
        static mutex mtx;
        vector<thread> threads;

        ThreadPool(){}

    public: 
        static ThreadPool* getInstance()
        {
            lock_guard<mutex> lock(mtx);
            if(instance == nullptr)
            {
                instance = new ThreadPool();
            }
            return instance;
        }
        
        void addTask(void (*task)())
        {
            threads.emplace_back(task);
        }

        void joinAll()
        {
            for(thread& t:threads)
            {
                if(t.joinable())
                {
                    t.join();
                }
            }
        }
};

ThreadPool* ThreadPool :: instance = nullptr;
mutex ThreadPool :: mtx;
mutex m;

void sampleTask()
{   
    lock_guard<mutex> lock(m);
    cout << "Task executed by thread" << this_thread :: get_id() <<endl;
}
int main()
{
    ThreadPool* pool = ThreadPool::getInstance();
    pool->addTask(sampleTask);
    pool->addTask(sampleTask);
    pool->joinAll();
    return 0;
}