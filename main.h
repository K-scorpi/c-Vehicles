#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <random>

// Abstract Car class
class Car {
protected:
    std::string brand;
    std::string model;
    int year;
    double price;

public:
    Car(const std::string& brand, const std::string& model, int year, double price)
        : brand(brand), model(model), year(year), price(price) {}
    virtual ~Car() {}
    virtual void drive() const = 0;
    virtual void display() const {
        std::cout << "Brand: " << brand << ", Model: " << model << ", Year: " << year << ", Price: $" << price << std::endl;
    }
    int getYear() const { return year; }
};

class SportsCar : public Car {
private:
    double topSpeed;
public:
    SportsCar(const std::string& brand, const std::string& model, int year, double price, double topSpeed)
        : Car(brand, model, year, price), topSpeed(topSpeed) {}
    void drive() const override {
        std::cout << "Driving the sports car at top speed of " << topSpeed << " km/h!" << std::endl;
    }
    void display() const override {
        Car::display();
        std::cout << "Top Speed: " << topSpeed << " km/h" << std::endl;
    }
};

class SUV : public Car {
private:
    double groundClearance;
public:
    SUV(const std::string& brand, const std::string& model, int year, double price, double groundClearance)
        : Car(brand, model, year, price), groundClearance(groundClearance) {}
    void drive() const override {
        std::cout << "Driving the SUV with a ground clearance of " << groundClearance << " inches!" << std::endl;
    }
    void display() const override {
        Car::display();
        std::cout << "Ground Clearance: " << groundClearance << " inches" << std::endl;
    }
};

class CarFactory {
public:
    virtual ~CarFactory() {}
    virtual std::shared_ptr<Car> createCar() const = 0;
};

class SportsCarFactory : public CarFactory {
public:
    std::shared_ptr<Car> createCar() const override;
};

class SUVFactory : public CarFactory {
public:
    std::shared_ptr<Car> createCar() const override;
};

class CarContainerIterator {
public:
    virtual ~CarContainerIterator() {}
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual std::shared_ptr<Car> current() const = 0;
};

class CarContainer {
public:
    virtual ~CarContainer() {}
    virtual void addCar(std::shared_ptr<Car> car) = 0;
    virtual std::unique_ptr<CarContainerIterator> createIterator() const = 0;
};

class VectorCarContainer : public CarContainer {
private:
    std::vector<std::shared_ptr<Car>> cars;
public:
    void addCar(std::shared_ptr<Car> car) override;
    std::unique_ptr<CarContainerIterator> createIterator() const override;
};

class IteratorDecorator : public CarContainerIterator {
protected:
    std::unique_ptr<CarContainerIterator> baseIterator;
public:
    IteratorDecorator(std::unique_ptr<CarContainerIterator> iterator) : baseIterator(std::move(iterator)) {}
    void first() override { baseIterator->first(); }
    void next() override { baseIterator->next(); }
    bool isDone() const override { return baseIterator->isDone(); }
    std::shared_ptr<Car> current() const override { return baseIterator->current(); }
};

class YearFilterDecorator : public IteratorDecorator {
    int filterYear;
public:
    YearFilterDecorator(std::unique_ptr<CarContainerIterator> iterator, int year)
        : IteratorDecorator(std::move(iterator)), filterYear(year) {}
    void first() override;
    void next() override;
};

class ListCarContainer : public CarContainer {
private:
    std::list<std::shared_ptr<Car>> cars;
public:
    void addCar(std::shared_ptr<Car> car) override;
    std::unique_ptr<CarContainerIterator> createIterator() const override;
};

class VectorCarContainerIterator : public CarContainerIterator {
private:
    const std::vector<std::shared_ptr<Car>>& cars;  // Ссылка на вектор автомобилей
    std::vector<std::shared_ptr<Car>>::const_iterator currentIterator;
    std::vector<std::shared_ptr<Car>>::const_iterator end;

public:
    VectorCarContainerIterator(const std::vector<std::shared_ptr<Car>>& cars) 
        : cars(cars), currentIterator(cars.begin()), end(cars.end()) {}

    void first() override { currentIterator = cars.begin(); } // Используем begin от вектора
    void next() override { if (currentIterator != end) ++currentIterator; }
    bool isDone() const override { return currentIterator == end; }
    std::shared_ptr<Car> current() const override { return (currentIterator != end) ? *currentIterator : nullptr; }
};


class ListCarContainerIterator : public CarContainerIterator {
private:
    const std::list<std::shared_ptr<Car>>& cars;  // Ссылка на список автомобилей
    std::list<std::shared_ptr<Car>>::const_iterator currentIterator;
    std::list<std::shared_ptr<Car>>::const_iterator end;

public:
    ListCarContainerIterator(const std::list<std::shared_ptr<Car>>& cars) 
        : cars(cars), currentIterator(cars.begin()), end(cars.end()) {}

    void first() override { currentIterator = cars.begin(); } // Используем begin от списка
    void next() override { if (currentIterator != end) ++currentIterator; }
    bool isDone() const override { return currentIterator == end; }
    std::shared_ptr<Car> current() const override { return (currentIterator != end) ? *currentIterator : nullptr; }
};

#endif // MAIN_H
