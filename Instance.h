#ifndef INSTANCE_H
#define INSTANCE_H

#include "Hardware.h"
#include <set>
#include <iostream>

class Instance {
private:
    int id;
    const Hardware* hardwarePtr;
    std::set<int> taskSet;
    bool is_Virtual;

public:
    Instance(int _id, const Hardware* _hardwarePtr);
    Instance(int _id, const Hardware* _hardwarePtr, bool _isVirtual);

    int getID() const;
    const Hardware* getHardwarePtr() const;
    bool isVirtual() const;

    bool operator<(const Instance& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Instance& instance);
    void setTasksSet(std::set<int> tasks);
    const std::set<int>& getTaskSet() const;
    void addTask(int task);
    void removeTask(int task);
};

#endif // INSTANCE_H
