#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <sqlite3.h>

class DatabaseManager
{
private:
    sqlite3* db;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool open(const std::string& dbName);
    void close();
    sqlite3* getDb() const;

    bool beginTransaction();
    bool commitTransaction();
};

#endif
