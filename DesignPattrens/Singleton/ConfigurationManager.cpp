#include<iostream>
#include <mutex>
#include <map>
using namespace std;

class ConfigurationManager
{
    private:
    static ConfigurationManager* instance;
    static mutex mtx;
    map<string,string> settings;

    ConfigurationManager(){}

    public:
    static ConfigurationManager* getInstance()
    {
        lock_guard<mutex> lock(mtx);
        if(instance == nullptr)
        {
            instance = new ConfigurationManager();
        }
        return instance;
    }

    void set(const string& key, const string& value)
    {
        settings[key] = value;
    }

    string get(const std::string& key)
    {
        return settings[key];
    }

    ~ConfigurationManager(){}
};

ConfigurationManager* ConfigurationManager::instance = nullptr;
mutex ConfigurationManager::mtx;

int main()
{
    // Get the singleton instance of ConfigurationManager
    ConfigurationManager* config = ConfigurationManager::getInstance();

    // Set configuration values
    config->set("AppName", "SingletonDemo");
    config->set("Version", "1.0.0");

    // Retrieve and print configuration values
    cout << "AppName: " << config->get("AppName") << endl;
    cout << "Version: " << config->get("Version") << endl;

    // Return success
    return 0;
}