#include<iostream>
#include <vector>
#include<unordered_map>
#include<memory>
#include <mutex>
#include<algorithm>
using namespace std;

class Notification 
{
    public:
    virtual void send(const string& message) = 0;
    virtual ~Notification() = default ;
};

class SMSNotification :public  Notification
{
    
    public:
    SMSNotification()
    {
        cout<<"SMSNotification initiated"<<endl;
    }
    void send(const string& message)
    {
        cout<<message<<endl;
    }
};

class EmailNotification : public Notification
{
    public:
    EmailNotification()
    {
        cout<<"EmailNotification initiated"<<endl;
    }
    void send(const string& message)
    {
        cout<<message<<endl;
    }
};

class PushNotification : public Notification
{
    public:
    PushNotification()
    {
        cout<<"PushNotification initiated"<<endl;
    }
    void send(const string& message)
    {
        cout<<message<<endl;
    }
};

class NotificationFactory
{
    public:
    static unique_ptr<Notification> createNotification(const string& format)
    {
        if(format == "SMS")
        return make_unique<SMSNotification>();
        else if(format == "EMAIL")
        return make_unique<EmailNotification>();
        else if(format == "PUSH")
        return make_unique<PushNotification>();
        else
        throw invalid_argument("Invalid notification format");
    }
};

class Observer
{
    public:
    virtual void update(const string& subscriptionType, const string&format,const string& message) = 0 ;
    virtual ~Observer(){}

};

class Subscriber : public Observer 
{
public:
    Subscriber(const string& name,const string& notificationType):name(name)
                                                                , notificationType(notificationType)
    {}

    void update(const string& subscriptionType, const string& format, const string& message)
    {
        auto formats = NotificationFactory:: createNotification(format);
        cout<< subscriptionType << " notification via " << notificationType <<endl;
        formats->send(message);
    }

private:
    string name;
    string notificationType;
};

// class Subject 
// {
//     public:
//     virtual void addSubscriber(const string& subscrption,Observer* subscriber) = 0;
//     virtual void removeSubscriber(const string& subscrption,Observer* subscriber) = 0;
//     virtual void notifySubscriber(const string& subscription, const string& message) = 0;

// };

class Subscribe 
{
    public:
    void addSubscriber(const string& subscription,Observer* subscriber)
    {   
        
        subscribers[subscription].push_back(subscriber);
    }

    void removeSubscriber(const string& subscription,Observer* subscriber)
    {
        auto& s = subscribers[subscription];
        s.erase(std::remove(s.begin(),s.end(),subscriber),s.end());
    }

    void notifySubscriber(const string& subscription, const string& format,const string& message)
    {
        for(Observer* obs : subscribers[subscription])
        {
            obs->update(subscription,format,message);
        }
    }

    private:
    unordered_map <string ,vector<Observer*>> subscribers;
}; 





class NotificationManager : public Subscribe
{
    public:
    static NotificationManager* getinstance()
    {
        
        lock_guard<mutex> lock(mtx);
        if(instance == nullptr)
        {
            instance = new NotificationManager();
        }
        return instance;
    }

    void sendNotfication(const string& subscription, const string& format, const string& message)
    {
        notifySubscriber(subscription,format,message);
        
    }

    ~NotificationManager()
    {
        if(instance != nullptr)
        {
            delete instance;
        }
    }

    private:
    NotificationManager(){};
    
    static NotificationManager * instance;
    static mutex mtx;
};

NotificationManager *NotificationManager::instance = nullptr;
mutex NotificationManager:: mtx;


int main()
{
  NotificationManager*  manager = NotificationManager::getinstance();
  Subscriber* adarsh = new Subscriber("adarsh", "SMS");
  Subscriber* rahul = new Subscriber("rahul", "EMAIL");

  manager->addSubscriber("Weather", adarsh);
  manager->sendNotfication("Weather","SMS","Rainy day ahead!");

  manager->addSubscriber("Stock", rahul);
  manager->sendNotfication("Stock","EMAIL","Stock market prize is increased drastically");


    return 0;
}