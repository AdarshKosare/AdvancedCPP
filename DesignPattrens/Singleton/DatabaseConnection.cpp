#include<iostream>
#include <mutex>
using namespace std;

class DatabaseConnection {
    private:
        static DatabaseConnection* instance;
        static mutex mtx;
        DatabaseConnection()
        {
            cout << "Database Connection is Established" << endl;
        }

    public:
        static DatabaseConnection* getInstance()
        {
            lock_guard<mutex> lock(mtx);
            if(instance == nullptr)
            {
                instance = new DatabaseConnection();
            }
            return instance;
        }

        void query(const string& sql)
        {
            cout << "Executing SQL: " << sql << endl;
        }
    };

    DatabaseConnection* DatabaseConnection :: instance = nullptr;
    mutex DatabaseConnection :: mtx;

int main()
{
    DatabaseConnection* db = DatabaseConnection::getInstance();
    db->query("SELECT * FROM users");
    db->query("INSERT INTO logs (message) VALUES ('Test Log')");
    return 0;
}