#include <iostream>
#include <string>
#include "sqlite3.h"
#include "main.h"

using namespace std;

// определение классов
class Cabriolet : public Car
{
    public:
        BodyType GetTypeOfCar() const {return BodyType::Cabriolet;}
};

class Coupe : public Car
{
    public:
        BodyType GetTypeOfCar() const {return BodyType::Coupe;}
};

class Pickup : public Car
{
    public:
        BodyType GetTypeOfCar() const {return BodyType::Pickup;}
};

class Sedan : public Car
{
    public:
        BodyType GetTypeOfCar() const {return BodyType::Sedan;}
};

//prints
string PrintBrand(const Brand brand)
{
    switch (brand)
    {
        case Brand::Volvo: return "ВОЛЬВО";
        case Brand::Mersedes: return "МЕРСЕДЕС";
        case Brand::BMW: return "BMW";
        case Brand::Skoda: return "ШКОДА";
    };
};

string PrintBodyType(BodyType type)
{
    switch(type)
    {
        case BodyType::Sedan: return "СЕДАН";
        case BodyType::Pickup: return "ПИКАП";
        case BodyType::Coupe: return "КУПЕ";
        case BodyType::Cabriolet: return "КАБРИОЛЕТ";
    };
};

string PrintPrice(Price price)
{
    switch(price)
    {
        case Price::Very_Low: return "ОЧЕНЬ ДЕШЕВАЯ";
        case Price::Low: return "ДЕШЕВАЯ";
        case Price::Medium: return "СРЕДНЯЯ ПО РЫНКУ";
        case Price::High: return "ДОРОГО";
        case Price::Very_High: return "ОЧЕНЬ ДОРОГО";
    };
};

//factory method
Car *CarFactory(BodyType newcar)
{
    switch(newcar)
    {
        case BodyType::Sedan: return new Sedan;
        case BodyType::Pickup: return new Pickup;
        case BodyType::Coupe: return new Coupe;
        case BodyType::Cabriolet: return new Cabriolet;
    }
};
// container methods

void DBCarContainer::ClearDB()
{
    char *errmsg;
    sqlite3_exec(DB,"DELETE FROM Cars", NULL, NULL, &errmsg);
};

void DBCarContainer::AddCar(CarPointer newCar)
{
    sqlite3_stmt* stmt;
    string bodytype = PrintBodyType(newCar->GetTypeOfCar());
    string brandofcar = PrintBrand(newCar->GetBrandOfCar());
    string priceofcar = PrintPrice(newCar->GetPriceOfCar());
    int probegofcar = newCar->GetProbegOfCar();
    string sql = "INSERT INTO Cars (BodyType,Brand,Price,Probeg)"
                            "VALUES (:type,:brand,:price, :probeg);";
    sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":type"), bodytype.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":brand"), brandofcar.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, ":price"), priceofcar.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":probeg"), probegofcar);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

};

//iterator methods

void DBCarContainerIterator::First()
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT ID FROM Cars ORDER BY ID ASC LIMIT 1;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    rc = sqlite3_step(stmt);
    CurrentId = sqlite3_column_int(stmt, 0);
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
