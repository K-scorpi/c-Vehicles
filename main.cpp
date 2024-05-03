#include <iostream>
#include <vector>
#include <list>
#include "main.h"

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
    CarVectorContainer CarPark;
    /*
    for (int i = 0; i<15; i++)
    {
        CarPark.AddCar(new Sedan);
    };
    for (int i = 0; i<15; i++ )
    {
        CarPark.AddCar(new Cabriolet);
    }
    */
    int random_cars = random()%(200-15+1)+1;
    cout << "Создаем " << random_cars << " машин" << "\n"; 
    for (int i=0; i<random_cars; i++)
    {
        CarPark.AddCar(CarFactory(BodyType(rand()%4)));
    }
    Iterator<CarPointer> *it = new DecoratorPrice(new DecoratorBrand(CarPark.GetIterator(), Brand::BMW), Price::Very_Low);
    Task(it);
};