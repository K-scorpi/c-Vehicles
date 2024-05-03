#include "main.h"
#include <random>

std::shared_ptr<Car> SportsCarFactory::createCar() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> yearDist(2000, 2022);
    std::uniform_real_distribution<double> priceDist(50000, 150000);
    std::uniform_real_distribution<double> speedDist(200, 300);
    return std::make_shared<SportsCar>("Ferrari", "F8", yearDist(gen), priceDist(gen), speedDist(gen));
}

std::shared_ptr<Car> SUVFactory::createCar() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> yearDist(2000, 2022);
    std::uniform_real_distribution<double> priceDist(35000, 85000);
    std::uniform_real_distribution<double> clearanceDist(10, 20);
    return std::make_shared<SUV>("Land Rover", "Defender", yearDist(gen), priceDist(gen), clearanceDist(gen));
}

void VectorCarContainer::addCar(std::shared_ptr<Car> car) {
    cars.push_back(car);
}

std::unique_ptr<CarContainerIterator> VectorCarContainer::createIterator() const {
    return std::make_unique<VectorCarContainerIterator>(cars);
}

void ListCarContainer::addCar(std::shared_ptr<Car> car) {
    cars.push_back(car);
}

std::unique_ptr<CarContainerIterator> ListCarContainer::createIterator() const {
    return std::make_unique<ListCarContainerIterator>(cars);
}

void YearFilterDecorator::first() {
    baseIterator->first();
    while (!baseIterator->isDone() && baseIterator->current()->getYear() != filterYear) {
        baseIterator->next();
    }
}

void YearFilterDecorator::next() {
    baseIterator->next();
    while (!baseIterator->isDone() && baseIterator->current()->getYear() != filterYear) {
        baseIterator->next();
    }
}

int main() {
    VectorCarContainer container;
    SportsCarFactory factory;
    container.addCar(factory.createCar());
    container.addCar(factory.createCar());

    auto it = container.createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        it->current()->display();
    }

    return 0;
}
