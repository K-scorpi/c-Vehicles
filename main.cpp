#include <iostream>
#include <string>
#include <cstdlib>
#include "sqlite3.h"
#include "main.h"

using namespace std;

// Define concrete car types by extending the Car base class
class Cabriolet : public Car {
public:
    BodyType GetTypeOfCar() const override { return BodyType::Cabriolet; }
};

class Coupe : public Car {
public:
    BodyType GetTypeOfCar() const override { return BodyType::Coupe; }
};

class Pickup : public Car {
public:
    BodyType GetTypeOfCar() const override { return BodyType::Pickup; }
};

class Sedan : public Car {
public:
    BodyType GetTypeOfCar() const override { return BodyType::Sedan; }
};

// Functions to print enums in a human-readable format
string PrintBrand(const Brand brand) {
    switch (brand) {
        case Brand::Volvo: return "ВОЛЬВО";
        case Brand::Mercedes: return "МЕРСЕДЕС"; 
        case Brand::BMW: return "BMW";
        case Brand::Skoda: return "ШКОДА";
        default: return "Неизвестный бренд";
    }
}

string PrintBodyType(const BodyType type) {
    switch (type) {
        case BodyType::Sedan: return "СЕДАН";
        case BodyType::Pickup: return "ПИКАП";
        case BodyType::Coupe: return "КУПЕ";
        case BodyType::Cabriolet: return "КАБРИОЛЕТ";
        default: return "Неизвестный тип";
    }
}

string PrintPrice(const Price price) {
    switch (price) {
        case Price::Very_Low: return "ОЧЕНЬ ДЕШЕВАЯ";
        case Price::Low: return "ДЕШЕВАЯ";
        case Price::Medium: return "СРЕДНЯЯ ПО РЫНКУ";
        case Price::High: return "ДОРОГАЯ";
        case Price::Very_High: return "ОЧЕНЬ ДОРОГАЯ";
        default: return "Неизвестная цена";
    }
}

// Factory method to create cars based on BodyType
Car* CarFactory(const BodyType type) {
    switch (type) {
        case BodyType::Sedan: return new Sedan();
        case BodyType::Pickup: return new Pickup();
        case BodyType::Coupe: return new Coupe();
        case BodyType::Cabriolet: return new Cabriolet();
        default: throw invalid_argument("Unknown BodyType");
    }
}

// Class methods for managing car database
void DBCarContainer::ClearDB() {
    char *errmsg;
    if (sqlite3_exec(DB, "DELETE FROM Cars", NULL, NULL, &errmsg) != SQLITE_OK) {
        cerr << "Error clearing database: " << errmsg << endl;
        sqlite3_free(errmsg);
    }
}

void DBCarContainer::AddCar(CarPointer newCar) {
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO Cars (BodyType, Brand, Price, Probeg) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "SQL error: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, PrintBodyType(newCar->GetTypeOfCar()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, PrintBrand(newCar->GetBrandOfCar()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, PrintPrice(newCar->GetPriceOfCar()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, newCar->GetProbegOfCar());

    // Execute and finalize statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Insert failed: " << sqlite3_errmsg(DB) << endl;
    }
    sqlite3_finalize(stmt);
}

// Implementation for DBCarContainerIterator methods
void DBCarContainerIterator::First() {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, "SELECT ID FROM Cars ORDER BY ID ASC LIMIT 1", -1, &stmt, nullptr) == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        CurrentId = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
};

int DBCarContainerIterator::GetCount()
{
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(DB, "SELECT COUNT(*) FROM Cars", -1, &stmt, 0);
    result = sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
};

string DBCarContainerIterator::GetBrand()
{
    sqlite3_stmt* stmt;
    const char *sql_brand = "SELECT Brand FROM Cars WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql_brand, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    string OurBrand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return OurBrand;
};

string DBCarContainerIterator::GetType()
{
    sqlite3_stmt* stmt;
    const char *sql_type = "SELECT BodyType FROM Cars WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql_type, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    string OurType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return OurType;
};

int DBCarContainerIterator::GetProbeg()
{
    sqlite3_stmt* stmt;
    const char *sql_probeg = "SELECT Probeg FROM Cars WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql_probeg, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    int OurProbeg = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return OurProbeg;
};

string DBCarContainerIterator::GetPrice()
{
    sqlite3_stmt* stmt;
    const char *sql_price = "SELECT Price FROM Cars WHERE ID = ?;";
    int rc = sqlite3_prepare_v2(DB, sql_price, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, CurrentId);
    rc = sqlite3_step(stmt);
    string OurPrice = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return OurPrice;
};

int main()
{
    srand(time(NULL));
    DBCarContainer parking("car.db");
    parking.ClearDB();
    int random_amount_of_cars = random()%(100-15+1)+2;
    cout << "Генерируем " << random_amount_of_cars << " машин" << "\n";
    for (int i=0; i<random_amount_of_cars; i++)
    {
        parking.AddCar(CarFactory(BodyType(rand()%3)));
    };
    DBCarContainerIterator it(parking.GetDB());

    it.First();
    for (int i = 0; i<it.GetCount(); i++)
    {
        Decorator check_brand(it.GetBrand());
        check_brand.Find("ВОЛЬВО");
        if (check_brand.GetCorrect() == true)
        {
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
            cout << "Тип машины: " << it.GetType() << "\n";
            cout << "Бренд: " << it.GetBrand() << "\n";
            cout << "Стоимость: " << it.GetPrice() << "\n";
            cout << "Пробег: " << it.GetProbeg() << " км\n";

        }
        it.Next();
    }
}
