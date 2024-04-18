#ifndef MEMORYCONTAINER_H
#define MEMORYCONTAINER_H

#include "BaseContainer.h"

// Контейнер, хранящий объекты в памяти
template <typename T>
class MemoryContainer : public BaseContainer<T> {
private:
    std::vector<T> data;

public:
    void insert(const T &obj) override {
        data.push_back(obj);
    }

    void remove(const T &obj) override {
        auto it = std::find(data.begin(), data.end(), obj);
        if (it != data.end()) {
            data.erase(it);
        }
    }

    void displayAll() const override {
        for (const auto &obj : data) {
            std::cout << obj << std::endl;
        }
    }

    void clear() override {
        data.clear();
    }

    int size() const override {
        return data.size();
    }

    bool isEmpty() const override {
        return data.empty();
    }
};

#endif // MEMORYCONTAINER_H
