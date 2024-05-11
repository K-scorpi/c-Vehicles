#ifndef CARS_H
#define CARS_H

#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"

using namespace std;

template<class Type>
class Iterator
{
    protected:
        Iterator() {}
    public:
        virtual ~Iterator(){};
        virtual void First() = 0 ;
        virtual void Next() = 0;
        virtual bool IsDone() const = 0;
        virtual Type GetCurrent() = 0;
};

enum class BodyType : int {Sedan, Pickup, Coupe, Cabriolet};

enum class Brand : int {Volvo, Mercedes, BMW, Skoda};

enum class Price : int {Very_Low, Low, Medium, High, Very_High};


class Car
{
    protected:
        BodyType TypeOfCar;
        Brand BrandOfCar;
        Price PriceOfCar;
        int Probeg;
        Car(int id_of_brand = rand()%4, int id_of_price = rand()%5, int probeg = rand()/10000)
        {
            PriceOfCar = Price(id_of_price);
            BrandOfCar = Brand(id_of_brand);
            Probeg = probeg;
        };
    public:
        virtual BodyType GetTypeOfCar() const = 0;
        Brand GetBrandOfCar() {return BrandOfCar;};
        Price GetPriceOfCar() {return PriceOfCar;};
        int GetProbegOfCar() {return Probeg;}
};

typedef Car * CarPointer;

class Cabriolet : public Car
{
    public:
        Cabriolet(int id_of_brand = rand()%4, int id_of_price = rand()%5, int probeg = rand()/10000) : Car(id_of_brand, id_of_price, probeg)
        {
            TypeOfCar = BodyType::Cabriolet;
        }
        BodyType GetTypeOfCar() const {return TypeOfCar;}
};

class Coupe : public Car
{
    public:
        Coupe(int id_of_brand = rand()%4, int id_of_price = rand()%5, int probeg = rand()/10000) : Car(id_of_brand, id_of_price, probeg)
        {
            TypeOfCar = BodyType::Coupe;
        }
        BodyType GetTypeOfCar() const {return TypeOfCar;}
};

class Pickup : public Car
{
    public:
        Pickup(int id_of_brand = rand()%4, int id_of_price = rand()%5, int probeg = rand()/10000) : Car(id_of_brand, id_of_price, probeg)
        {
            TypeOfCar = BodyType::Pickup;
        }
        BodyType GetTypeOfCar() const {return TypeOfCar;}
};

class Sedan : public Car
{
    public:
        Sedan(int id_of_brand = rand()%4, int id_of_price = rand()%5, int probeg = rand()/10000) : Car(id_of_brand, id_of_price, probeg)
        {
            TypeOfCar = BodyType::Sedan;
        }
        BodyType GetTypeOfCar() const {return TypeOfCar;}
};

class CarContainer
{
    public:
        virtual void AddCar(CarPointer newcar) = 0;
        virtual int GetCount() =0;
};

class CarDbIterator : public Iterator<CarPointer>
{
    private:
        sqlite3* DB;
        int FirstId;
        int Count;
        int PosDB;
    public:
        CarDbIterator(sqlite3* db, int count)
        {
            DB = db;
            Count = count;
        };
        void First() {PosDB = 0;};
        void Next() {PosDB+=1;}
        bool IsDone() const {return PosDB >= Count;};
        CarPointer GetCurrent();
};

class CarDbContainer : public CarContainer
{
    private:
        sqlite3* DB;
        int CarInDB;
    public:
        CarDbContainer(const string& DB_path)
        {
            sqlite3_open(DB_path.c_str(), &DB);
            string createtable = "CREATE TABLE Cars ("
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "BodyType INTEGER,"
                                "Brand INTEGER,"
                                "Price INTEGER,"
                                "Probeg INTEGER"
                                ");";
            char *errMsg;
            sqlite3_exec(DB, createtable.c_str(), nullptr, nullptr, &errMsg);
            cout << errMsg << "\n";
            CarInDB = 0;
        }
        void AddCar(CarPointer newCar);
        int GetCount() { return CarInDB;}
        void ClearDB();
        Iterator<CarPointer> * GetIterator()
        {
            return new CarDbIterator(DB, CarInDB);
        };
};
class CarListIterator : public Iterator<CarPointer>
{
    private:
        const CarPointer *CarPark;
        int Pos;
        int Count;
    public:
        CarListIterator(const CarPointer *carpark, int count)
        {
            CarPark = carpark;
            Count = count;
            Pos = 0;
        };
        void First() { Pos = 0; }
        void Next() {Pos++;}
        bool IsDone() const {return Pos >= Count;}
        CarPointer GetCurrent() { return CarPark[Pos];}
};

class CarListContainer : public CarContainer
{
    private:
        CarPointer * CarPark;
        int CarCount;
        int MaxSize;
    public:
        CarListContainer(int MaxSize);
        virtual ~CarListContainer();
        void AddCar(CarPointer newcar);
        int GetCount()  {return CarCount;}
        CarPointer GetByIndex(int index) const {return CarPark[index];}
        Iterator<CarPointer> * GetIterator()
        {
            return new CarListIterator(CarPark, CarCount);
        };
};

class CarVectorIterator : public Iterator<CarPointer>
{
    private:
        const vector<CarPointer> * CarPark;
        vector<CarPointer>::const_iterator it;
    public:
        CarVectorIterator( const vector<CarPointer> * carpark)
        {
            CarPark = carpark;
            it = CarPark->begin();
        };
        void First() { it = CarPark->begin();}
        void Next() {it++;}
        bool IsDone() const {return it == CarPark->end();}
        CarPointer GetCurrent() {return *it;}

};

class CarVectorContainer : public CarContainer
{
    private:
        vector<CarPointer> CarPark;
    public:
        void AddCar(CarPointer newcar) {CarPark.push_back(newcar);}
        int GetCount() {return CarPark.size();}
        Iterator<CarPointer> * GetIterator()
        {
            return new CarVectorIterator(&CarPark);
        };
};

template<class Type>
class Decorator : public Iterator<Type>
{
    protected:
        Iterator<Type> *It;
    public:
        Decorator (Iterator<Type> *it)
        {
            It = it;
        }
        virtual ~Decorator() {delete It;}
        void First() {It->First();}
        void Next() {It->Next();}
        bool IsDone() const {return It->IsDone();}
        Type GetCurrent() { return It->GetCurrent(); }
};

class DecoratorBrand : public Decorator<CarPointer>
{
    private:
        Brand TargetBrand;
    public:
        DecoratorBrand(Iterator<CarPointer> *it, Brand targetbrand) : Decorator(it)
        {
            TargetBrand = targetbrand;
        };
        void First()
        {
            It->First();
            while(!It->IsDone() && It->GetCurrent()->GetBrandOfCar() != TargetBrand)
            {
                It->Next();
            }
        };
        void Next()
        {
            do
            {
                It->Next();

            }while(!It->IsDone() && It->GetCurrent()->GetBrandOfCar()!= TargetBrand);
        };
};

class DecoratorPrice : public Decorator<CarPointer>
{
    private:
        Price TargetPrice;
    public:
        DecoratorPrice(Iterator<CarPointer> *it, Price targetprice) : Decorator(it)
        {
            TargetPrice = targetprice;
        };
        void First()
        {
            It->First();
            while(!It->IsDone() && It-> GetCurrent()->GetPriceOfCar() != TargetPrice)
            {
                It->Next();
            }
        };
        void Next()
        {
            do
            {
                It->Next();

            }while(!It->IsDone() && It->GetCurrent()->GetPriceOfCar()!= TargetPrice);
        };
};

class DecoratorType : public Decorator<CarPointer>
{
    private:
        BodyType TargetType;
    public:
        DecoratorType(Iterator<CarPointer> *it, BodyType targettype) : Decorator(it)
        {
            TargetType = targettype;
        };
        void First()
        {
            It->First();
            while(!It->IsDone() && It->GetCurrent()->GetTypeOfCar() != TargetType)
            {
                It->Next();
            }
        };
        void Next()
        {
            do
            {
                It->Next();

            }while(!It->IsDone() && It->GetCurrent()->GetTypeOfCar()!= TargetType);
        };
};

class DecoratorProbeg : public Decorator<CarPointer>
{
    private:
        int TargetProbeg;
    public:
        DecoratorProbeg(Iterator<CarPointer> *it, int targetprobeg) : Decorator(it)
        {
            TargetProbeg = targetprobeg;
        };
        void First()
        {
            It->First();
            while(!It->IsDone() && It-> GetCurrent()->GetProbegOfCar() > TargetProbeg)
            {
                It->Next();
            }
        };
        void Next()
        {
            do
            {
                It->Next();

            }while(!It->IsDone() && It->GetCurrent()->GetProbegOfCar() > TargetProbeg);
        };
};
#endif CARS_H