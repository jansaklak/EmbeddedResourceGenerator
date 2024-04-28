#include "Hardware.h"
#include <unordered_set>
#include <iostream>
#include "Times.h"
#include "Instance.h"

Instance::Instance(int _id, const Hardware* _hardwarePtr)
    : id(_id), hardwarePtr(_hardwarePtr){
        is_Virtual = 0;
    }

Instance::Instance(int _id, const Hardware* _hardwarePtr, bool _isVirtual)
    : id(_id), hardwarePtr(_hardwarePtr){
        is_Virtual = _isVirtual;
    }

int Instance::getID() const { 
    return id;
}

const Hardware* Instance::getHardwarePtr() const {
    return hardwarePtr;
}

bool Instance::isVirtual() const {
    if(this->is_Virtual==1) return 1;
    return 0;
}

bool Instance::operator<(const Instance& other) const {
    if (hardwarePtr != other.hardwarePtr) {
        return hardwarePtr < other.hardwarePtr;
    }
    return id < other.id;
}

std::ostream& operator<<(std::ostream& os, const Instance& instance) {
    if(instance.isVirtual()){
        os << "V_" << *instance.getHardwarePtr() << "_" << instance.getID();
    }
    else{
        os << *instance.getHardwarePtr() << "_" << instance.getID();
    }
    return os;
}

const std::set<int>& Instance::getTaskSet() const {
    return taskSet;
}

void Instance::setTasksSet(std::set<int> tasks){
    taskSet = tasks;
}

void Instance::addTask(int task) { 
    taskSet.insert(task); 
}

void Instance::removeTask(int task) { 
    taskSet.erase(task); 
}
