#include<iostream>
#include<sqlite3.h>
#include <mutex>
using namespace std;


class SQLiteDatabase{
    private:
        sqlite3* db;
        int rc;
        static SQLiteDatabase* instance;
        static mutex mtx;
        
        SQLiteDatabase(const string &db_name)
        {
            rc = sqlite3_open(db_name.c_str(),&db);
            if(rc)
            {
                cerr << "Can't Open Database: " << sqlite3_errmsg(db) << endl;
                db = nullptr;
            }
            else{
                cout << "Database opened successfully: " << db_name << endl;
            }
        }

        ~SQLiteDatabase()
        {
            if(db)
            {
                sqlite3_close(db);
                cout << "Database Closed" <<endl;
            }
        }
        
    public:
        SQLiteDatabase(const SQLiteDatabase&) = delete;
        SQLiteDatabase& operator =(const SQLiteDatabase&) = delete;


        static SQLiteDatabase* getInstance(const string db_name)
        {
            lock_guard <mutex> lock(mtx);
            if(!instance)
            {
                instance = new SQLiteDatabase(db_name);
            }
            return instance;
        }

        void execute(const string& sql)
        {
            char* errMsg = nullptr;
            rc = sqlite3_exec(db,sql.c_str(),nullptr,nullptr, &errMsg);
            if(rc != SQLITE_OK)
            {
                cerr << "SQL error: "<< errMsg << endl;
                sqlite3_free(errMsg);
            }
            else
            {
                cout <<"SQL executed Successfully" <<endl;
            }

        }

        void query(const string& sql)
        {
            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db,sql.c_str(), -1, &stmt, nullptr);
            if(rc != SQLITE_OK)
            {
                cerr<<"Failed to prepare statement: "<< sqlite3_errmsg(db) <<endl;
                return;
            }

            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                string column1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt,0));
                int column2 = sqlite3_column_int(stmt,1);

                cout << " Row: " << column1 << "|" << column2 << endl;
            }

            sqlite3_finalize(stmt);

        }
};

SQLiteDatabase* SQLiteDatabase :: instance = nullptr;
mutex SQLiteDatabase :: mtx;

int main()
{
    SQLiteDatabase* db = SQLiteDatabase :: getInstance("example.db");

    db->execute("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);");

    // Insert data
    db->execute("INSERT INTO users (name, age) VALUES ('Alice', 30);");
    db->execute("INSERT INTO users (name, age) VALUES ('Bob', 25);");

    // Query data
    db->query("SELECT name, age FROM users;");
    return 0;
}