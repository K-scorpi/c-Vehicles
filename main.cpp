#include <iostream>
#include <vector>
#include <list>
#include "main.h"
#include "sqlite3.h"

using namespace std;

void CarListContainer::AddCar(CarPointer newcar)
{
    CarPark[CarCount] = newcar;
    CarCount++;
}

CarListContainer::CarListContainer(int maxsize)
{
    CarPark = new CarPointer[maxsize];
    for (int i=0; i<maxsize; i++)
    {
        CarPark[i] = NULL;
    };
    CarCount = 0;
    MaxSize = maxsize;
};

CarListContainer::~CarListContainer()
{
    for (int i=0; i<MaxSize; i++)
    {
        if (CarPark[i] != NULL)
        {
            delete CarPark[i];
            CarPark[i] = NULL;
        };
    };
    delete[] CarPark;
};

string PrintBrand(const Brand brand)
{
    switch (brand)
    {
        case Brand::Volvo: return "ВОЛЬВО";
        case Brand::Mercedes: return "МЕРСЕДЕС";
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

void Task(Iterator<CarPointer> *it)
{
    for (it->First(); !it->IsDone(); it->Next())
    {
        const CarPointer currentcar = it->GetCurrent();
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n";
        cout << "ХАРАКТЕРИСТИКИ МАШИНЫ:" << "\n";
        cout << "- Производитель: " << PrintBrand(currentcar->GetBrandOfCar()) << "\n";
        cout << "- Тип кузова: " << PrintBodyType(currentcar->GetTypeOfCar()) << "\n";
        cout << "- Пробег: " << currentcar->GetProbegOfCar() << " км" <<"\n";
        cout << "- Стоимость: " << PrintPrice(currentcar->GetPriceOfCar()) << "\n";
    }
}

CarPointer CarDbIterator::GetCurrent()
{
    
    sqlite3_stmt* stmt;
    
    const char *sql = "SELECT ID FROM Cars ORDER BY ID ASC LIMIT 1;";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    rc = sqlite3_step(stmt);
    FirstId = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    CarPointer current;
    int needed_id = FirstId+PosDB;
    const char *get_data = "SELECT BodyType,Brand,Price,Probeg FROM Cars WHERE ID = ?;";
    rc = sqlite3_prepare_v2(DB, get_data, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, needed_id);
    rc = sqlite3_step(stmt);
    int type_id = sqlite3_column_int(stmt,0);
    int brand_id = sqlite3_column_int(stmt,1);
    int price_id = sqlite3_column_int(stmt, 2);
    int probeg_id = sqlite3_column_int(stmt, 3);
    switch(type_id)
    {
        case 0:
            current = new Sedan(brand_id, price_id, probeg_id);
            break;
        case 1:
            current = new Pickup(brand_id, price_id, probeg_id);
            break;
        case 2:
            current = new Coupe(brand_id, price_id, probeg_id);
            break;
        case 3:
            current = new Cabriolet(brand_id, price_id, probeg_id);
            break;
    }
    return current;
};

void CarDbContainer::ClearDB()
{
    char *errmsg;
    sqlite3_exec(DB,"DELETE FROM Cars", NULL, NULL, &errmsg);
};

void CarDbContainer::AddCar(CarPointer newCar)
{
    int cartype = (int)newCar->GetTypeOfCar(); 
    int carbrand= (int)newCar->GetBrandOfCar();
    int carprice = (int)newCar->GetPriceOfCar();
    int carprobeg = (int)newCar->GetProbegOfCar();
    sqlite3_stmt* stmt;
    string insert_query = "INSERT INTO Cars(BodyType,Brand,Price,Probeg)"
                            "VALUES (:body,:brand,:price,:probeg);";
    sqlite3_prepare_v2(DB, insert_query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":body"), cartype);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":brand"), carbrand);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":price"), carprice);
    sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, ":probeg"), carprobeg);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    CarInDB++;
};
Car *CarFactory(BodyType newcar)
{
    switch(newcar)
    {
        case BodyType::Sedan: return new Sedan;
        case BodyType::Pickup: return new Pickup;
        case BodyType::Coupe: return new Coupe;
        case BodyType::Cabriolet: return new Cabriolet;
    }
}
int main()
{
    srand(time(NULL));
    //CarListContainer CarPark(100);
    //CarVectorContainer CarPark;
    CarDbContainer CarPark("car.db");
    /*
    for (int i = 0; i<15; i++)
    {
        CarPark.AddCar(new Sedan);
    };
    for (int i = 0; i<15; i++ )
    {
        CarPark.AddCar(new Cabriolet);
    }*/
    int random_cars = random()%(200-15+1)+1;
    cout << "Создаем " << random_cars << " машин" << "\n"; 
    for (int i=0; i<random_cars; i++)
    {
        CarPark.AddCar(CarFactory(BodyType(rand()%4)));
    }
    Iterator<CarPointer> *it = new DecoratorProbeg(
        new DecoratorBrand(CarPark.GetIterator(),Brand::BMW), 150000
        );
    Task(it);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~