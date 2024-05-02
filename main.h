#ifndef VEHICLES_H
#define VEHICLES_H

#include <iostream>
#include <string>
using namespace std;

class Car {
protected:
    string brand; // марка
    string model; // модель
    int year;          // год выпуска
    float price;       // цена

public:
    Car(const string &brand, const string &model, int year, float price);
    virtual ~Car() {}

    virtual void displayInfo();
    virtual void accelerate() = 0; // Виртуальный метод для ускорения
    virtual void brake() = 0;      // Виртуальный метод для торможения
};

class PassengerCar : public Car {
protected:
    int passengers; // количество пассажиров
    string bodyType; // тип кузова

public:
    PassengerCar(const string &brand, const string &model, int year, float price,
                 int passengers, const string &bodyType);
    virtual ~PassengerCar() {}

    void displayInfo() override;
    void accelerate() override;
    void brake() override;
};

class Truck : public Car {
protected:
    float weight; // грузоподъемность

public:
    Truck(const string &brand, const string &model, int year, float price,
          float weight);
    virtual ~Truck() {}

    void displayInfo() override;
    void accelerate() override;
    void brake() override;
};

#endif // VEHICLES_H
