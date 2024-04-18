#include <iostream>
#include "main.h"
using namespace std;

//Car class
Car::Car(const string &brand, const string &model, int year, float price)
    : brand(brand), model(model), year(year), price(price) {}

void Car::displayInfo() {
    cout << "Brand: " << brand << endl;
    cout << "Model: " << model << endl;
    cout << "Year: " << year << endl;
    cout << "Price: $" << price << endl;
}

//PassengerCar class
PassengerCar::PassengerCar(const string &brand, const string &model, int year, float price,
                           int passengers, const string &bodyType)
    : Car(brand, model, year, price), passengers(passengers), bodyType(bodyType) {}

void PassengerCar::displayInfo() {
    Car::displayInfo();
    cout << "Passengers: " << passengers << endl;
    cout << "Body Type: " << bodyType << endl;
}

void PassengerCar::accelerate() {
    cout << "Passenger car is accelerating." << endl;
}

void PassengerCar::brake() {
    cout << "Passenger car is braking." << endl;
}

// Implementations for Truck class
Truck::Truck(const string &brand, const string &model, int year, float price,
             float weight)
    : Car(brand, model, year, price), weight(weight) {}

void Truck::displayInfo() {
    Car::displayInfo();
    cout << "weight: " << Weight << " tons" << endl;
}

void Truck::accelerate() {
    cout << "Truck is accelerating." << endl;
}

void Truck::brake() {
    cout << "Truck is braking." << endl;
}

int main() {
    PassengerCar car1("Toyota", "Camry", 2022, 25000, 5, "sedan");
    Truck truck1("Ford", "F-150", 2023, 35000, 2.5);

    car1.displayInfo();
    cout << endl;
    truck1.displayInfo();

    car1.accelerate();
    truck1.accelerate();

    return 0;
}
