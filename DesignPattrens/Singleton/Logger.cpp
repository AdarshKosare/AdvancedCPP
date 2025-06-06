#include <iostream>
#include <mutex>
#include <fstream>
using namespace std;

class Logger
{
private:
    static Logger *instance;
    static mutex mtx;
    ofstream logFile;

    Logger()
    {
        logFile.open("log.txt", ios::out | ios::app);
    }

public:
    // Returns the singleton instance of Logger
    static Logger *getInstance()
    {
        // Lock the mutex to ensure thread safety
        lock_guard<mutex> lock(mtx);
        // If instance does not exist, create it
        if (instance == nullptr)
        {
            instance = new Logger();
        }
        // Return the singleton instance
        return instance;
    }

    void log(const std::string &message)
    {
        logFile << message << std::endl;
    }

    ~Logger()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }
};

Logger *Logger::instance = nullptr;
mutex Logger::mtx;

int main()
{
    Logger *logger = Logger::getInstance();
    logger->log("Application started");
    logger->log("Another log entry");
    return 0;
}