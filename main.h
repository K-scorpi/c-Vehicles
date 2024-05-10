#ifndef CARS_SQL_H
#define CARS_SQL_H

#include "sqlite3.h"
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

// Enumerations to represent car attributes
enum class BodyType : int { Sedan, Pickup, Coupe, Cabriolet };
enum class Brand : int { Volvo, Mercedes, BMW, Skoda };
enum class Price : int { Very_Low, Low, Medium, High, Very_High };

// Abstract base class for Car
class Car {
protected:
    BodyType typeOfCar;
    Brand brandOfCar;
    Price priceOfCar;

    // Constructor initializes car with random attributes
    Car() : priceOfCar(Price(rand() % 5)), brandOfCar(Brand(rand() % 4)) {}

public:
    virtual ~Car() = default; // Virtual destructor for cleanup of derived types
    virtual BodyType GetTypeOfCar() const = 0;
    Brand GetBrandOfCar() const { return brandOfCar; }
    Price GetPriceOfCar() const { return priceOfCar; }

    // Returns a randomly generated mileage
    int GetProbegOfCar() const {
        return rand() / 10000;
    }
};

using CarPointer = Car*; // Type alias for pointer to Car

// Container class to manage database operations for Cars
class DBCarContainer {
private:
    sqlite3* db;

public:
    explicit DBCarContainer(const string& dbPath) {
        if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
            cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        }

        const string createTableSQL = R"(
            CREATE TABLE Cars (
                ID INTEGER PRIMARY KEY AUTOINCREMENT,
                BodyType TEXT NOT NULL,
                Brand TEXT,
                Price TEXT,
                Probeg INTEGER
            );
        )";
        char* errMsg = nullptr;
        if (sqlite3_exec(db, createTableSQL.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Error creating table: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

    ~DBCarContainer() {
        sqlite3_close(db);
    }

    void AddCar(CarPointer newCar);
    void ClearDB();
    sqlite3* GetDB() const { return db; }
};

// Iterator to navigate through the Cars in the database
class DBCarContainerIterator {
private:
    int currentId;
    sqlite3* db;

public:
    explicit DBCarContainerIterator(sqlite3* db) : db(db), currentId(0) {}

    void First();
    int GetCount();
    string GetBrand();
    string GetType();
    int GetProbeg();
    string GetPrice();
    void Next() { currentId++; }
};

// Decorator class for string comparison
class Decorator {
private:
    string target;
    string current;
    bool isCorrect;

public:
    explicit Decorator(const string& current) : current(current), isCorrect(true) {}

    void Find(const string& target) {
        this->target = target;
        isCorrect = (current == target);
    }

    bool GetCorrect() const { return isCorrect; }
};

#endif // CARS_SQL_H
