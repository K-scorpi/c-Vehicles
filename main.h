#ifndef CARS_H
#define CARS_H

#include <iostream>
#include <string>
#include <vector>

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
        virtual Type GetCurrent() const = 0;
};

enum class BodyType : int {Sedan, Pickup, Coupe, Cabriolet};

enum class Brand : int {Volvo, Mersedes, BMW, Skoda};

enum class Price : int {Very_Low, Low, Medium, High, Very_High};

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

class CarContainer
{
    public:
        virtual void AddCar(CarPointer newcar) = 0;
        virtual int GetCount() const =0;
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
        CarPointer GetCurrent() const { return CarPark[Pos];}
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
        int GetCount() const {return CarCount;}
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
        CarPointer GetCurrent() const {return *it;}

};

class CarVectorContainer : public CarContainer
{
    private:
        vector<CarPointer> CarPark;
    public:
        void AddCar(CarPointer newcar) {CarPark.push_back(newcar);}
        int GetCount() const {return CarPark.size();}
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
        Type GetCurrent() const { return It->GetCurrent(); }
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