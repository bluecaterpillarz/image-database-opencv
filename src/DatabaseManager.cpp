#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open(const std::string& dbName)
{
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "Database could not be opened: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void DatabaseManager::close()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

sqlite3* DatabaseManager::getDb() const
{
    return db;
}

bool DatabaseManager::beginTransaction()
{
    char* errorMessage = nullptr;
    int result = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errorMessage);

    if (result != SQLITE_OK)
    {
        std::cerr << "Transaction could not start: "
                  << (errorMessage ? errorMessage : "Unknown error") << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool DatabaseManager::commitTransaction()
{
    char* errorMessage = nullptr;
    int result = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errorMessage);

    if (result != SQLITE_OK)
    {
        std::cerr << "Commit failed: "
                  << (errorMessage ? errorMessage : "Unknown error") << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    std::cout << "Operation completed." << std::endl;
    return true;
}
