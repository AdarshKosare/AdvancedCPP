# SQLiteDatabase Singleton Pattern Example

This project demonstrates a thread-safe Singleton implementation for managing an SQLite database connection in C++. The `SQLiteDatabase` class ensures only one instance interacts with the database, providing methods to execute SQL commands and query data.

---

## Key Components

### 1. Singleton Pattern

- **Private Constructor & Destructor:**  
  Only accessible within the class to prevent direct instantiation or deletion.
- **Deleted Copy Operations:**  
  Copy constructor and assignment operator are deleted to prevent copying.
- **Static Instance & Mutex:**  
  Ensures only one instance exists and is thread-safe.

### 2. Database Operations

- **execute:**  
  Executes SQL commands (e.g., CREATE, INSERT).
- **query:**  
  Executes SELECT queries and prints results.

---

## Code Overview

```cpp
#include<iostream>
#include<sqlite3.h>
#include <mutex>
using namespace std;

class SQLiteDatabase {
private:
    sqlite3* db;
    int rc;
    static SQLiteDatabase* instance;
    static mutex mtx;

    // Constructor: Opens the database
    SQLiteDatabase(const string &db_name) {
        rc = sqlite3_open(db_name.c_str(), &db);
        if (rc) {
            cerr << "Can't Open Database: " << sqlite3_errmsg(db) << endl;
            db = nullptr;
        } else {
            cout << "Database opened successfully: " << db_name << endl;
        }
    }

    // Destructor: Closes the database
    ~SQLiteDatabase() {
        if (db) {
            sqlite3_close(db);
            cout << "Database Closed" << endl;
        }
    }

public:
    // Prevent copying
    SQLiteDatabase(const SQLiteDatabase&) = delete;
    SQLiteDatabase& operator=(const SQLiteDatabase&) = delete;

    // Singleton instance getter
    static SQLiteDatabase* getInstance(const string db_name) {
        lock_guard<mutex> lock(mtx);
        if (!instance) {
            instance = new SQLiteDatabase(db_name);
        }
        return instance;
    }

    // Execute SQL command
    void execute(const string& sql) {
        char* errMsg = nullptr;
        rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error: " << errMsg << endl;
            sqlite3_free(errMsg);
        } else {
            cout << "SQL executed Successfully" << endl;
        }
    }

    // Query and print results
    void query(const string& sql) {
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
            return;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string column1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int column2 = sqlite3_column_int(stmt, 1);
            cout << " Row: " << column1 << "|" << column2 << endl;
        }

        sqlite3_finalize(stmt);
    }
};

// Static member initialization
SQLiteDatabase* SQLiteDatabase::instance = nullptr;
mutex SQLiteDatabase::mtx;

// Main function demonstrating usage
int main() {
    SQLiteDatabase* db = SQLiteDatabase::getInstance("example.db");

    db->execute("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);");

    // Insert data
    db->execute("INSERT INTO users (name, age) VALUES ('Alice', 30);");
    db->execute("INSERT INTO users (name, age) VALUES ('Bob', 25);");

    // Query data
    db->query("SELECT name, age FROM users;");
    return 0;
}
```

---

## How It Works

1. **Singleton Instance:**  
   Use `SQLiteDatabase::getInstance("example.db")` to get the single instance.
2. **Executing SQL:**  
   Use `execute()` to run SQL commands (table creation, insertion).
3. **Querying Data:**  
   Use `query()` to fetch and print results from the database.

---

## Notes

- The singleton ensures only one database connection exists.
- Thread safety is provided by `std::mutex`.
- Error handling is included for database operations.

---