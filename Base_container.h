#ifndef BASECONTAINER_H
#define BASECONTAINER_H

#include <vector>

// Базовый класс контейнера
template <typename T>
class BaseContainer {
public:
    virtual ~BaseContainer() {}

    virtual void insert(const T &obj) = 0;
    virtual void remove(const T &obj) = 0;
    virtual void displayAll() const = 0;
    virtual void clear() = 0;
    virtual int size() const = 0;
    virtual bool isEmpty() const = 0;
};

#endif // BASECONTAINER_H
