#ifndef Cars_SQL_H

#include "sqlite3.h"
#include <string>

using namespace std;

//перечисления параметров
enum class BodyType : int {Sedan, Pickup, Coupe, Cabriolet};
enum class Brand : int {Volvo, Mercedes, BMW, Skoda};
enum class Price : int {Very_Low, Low, Medium, High, Very_High};
//общий класс
class Car
{
    protected:
        BodyType TypeOfCar;
        Brand BrandOfCar;
        Price PriceOfCar;
        Car()
        {
            PriceOfCar = Price(rand()%5);
            BrandOfCar = Brand(rand()%4);
        };
    public:
        virtual BodyType GetTypeOfCar() const = 0;
        Brand GetBrandOfCar() {return BrandOfCar;};
        Price GetPriceOfCar() {return PriceOfCar;};
        int GetProbegOfCar()
        {
            int probeg = rand()/10000;
            return probeg;
        };
};

typedef Car * CarPointer;

class DBCarContainer
{
    private:
        sqlite3* DB;
    public:
        DBCarContainer(const string& DB_path)
        {
            sqlite3_open(DB_path.c_str(), &DB);
            string createtable = "CREATE TABLE Cars ("
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "BodyType TEXT NOT NULL,"
                                "Brand TEXT,"
                                "Price TEXT,"
                                "Probeg INTEGER"
                                ");";
            char *errMsg;
            sqlite3_exec(DB, createtable.c_str(), nullptr, nullptr, &errMsg);
            cout << errMsg << "\n";
        };
        void AddCar(CarPointer newCar);
        void ClearDB();
        sqlite3* GetDB() {return DB;}
};

class DBCarContainerIterator
{
    private:
        int CurrentId;
        sqlite3* DB;
    public:
        DBCarContainerIterator(sqlite3* db)
        {
            DB = db;
        };
        void First();
        int GetCount();
        string GetBrand();
        string GetType();
        int GetProbeg();
        string GetPrice();
        void Next() {CurrentId++;};
};

class Decorator
{
    private:
        string Target;
        string Current;
        bool IsCorrect;
    public:
        Decorator(string current)
        {
            Current = current;
            IsCorrect  = true;
        };
        void Find(string target)
        {
            Target = target;
            if (Current == Target)
            {
                IsCorrect = true;
            }
            else
            {
                IsCorrect = false;
            }
        };
        bool GetCorrect() {return IsCorrect;}
};
#endif // Cars_SQL_H