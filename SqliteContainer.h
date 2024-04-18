#ifndef SQLITECONTAINER_H
#define SQLITECONTAINER_H

#include "BaseContainer.h"
#include "sqlite3.h"

// Контейнер, хранящий объекты в базе данных SQLite
template <typename T>
class SqliteContainer : public BaseContainer<T> {
private:
    sqlite3 *db;
    std::string tableName;

    static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
        }
        std::cout << std::endl;
        return 0;
    }

public:
    SqliteContainer(const std::string &dbFile, const std::string &table)
        : tableName(table) {
        int rc = sqlite3_open(dbFile.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
        } else {
            std::cout << "Opened database successfully" << std::endl;

            std::string sql = "CREATE TABLE IF NOT EXISTS " + tableName + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, DATA TEXT);";

            char *errMsg = nullptr;
            rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                std::cerr << "SQL error: " << errMsg << std::endl;
                sqlite3_free(errMsg);
            } else {
                std::cout << "Table created successfully" << std::endl;
            }
        }
    }

    ~SqliteContainer() {
        sqlite3_close(db);
    }

    void insert(const T &obj) override {
        std::string sql = "INSERT INTO " + tableName + " (DATA) VALUES ('" + obj + "');";

        char *errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "Record inserted successfully" << std::endl;
        }
    }

    void remove(const T &obj) override {
        std::string sql = "DELETE FROM " + tableName + " WHERE DATA = '" + obj + "';";

        char *errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "Record deleted successfully" << std::endl;
        }
    }

    void displayAll() const override {
        std::string sql = "SELECT * FROM " + tableName + ";";

        char *errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }

    void clear() override {
        std::string sql = "DELETE FROM " + tableName + ";";

        char *errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "All records deleted successfully" << std::endl;
        }
    }

    int size() const override {
        std::string sql = "SELECT COUNT(*) FROM " + tableName + ";";
        sqlite3_stmt *stmt;
        int result = -1;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                result = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        return result;
    }

    bool isEmpty() const override {
        return size() == 0;
    }
};

#endif // SQLITECONTAINER_H
