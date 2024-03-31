#include "Hardware.h"
#include <unordered_set>
#include <iostream>
#include "Times.h"
class Instance {
private:
    int id;
    const Hardware* hardwarePtr;
    std::unordered_set<int> taskSet;

public:
    Instance(int _id, const Hardware* _hardwarePtr)
        : id(_id), hardwarePtr(_hardwarePtr){}
    int getID() const { return id; }
    const Hardware* getHardwarePtr() const { return hardwarePtr; }

    bool operator<(const Instance& other) const {
        if (hardwarePtr != other.hardwarePtr) {
            return hardwarePtr < other.hardwarePtr;
        }
        return id < other.id;
    }
    friend std::ostream& operator<<(std::ostream& os, const Instance& instance) {
        os << *instance.getHardwarePtr() << "_" << instance.getID();
        return os;
    }
    
    const std::unordered_set<int>& getTaskSet() const { return taskSet; }
    void addTask(int task) { 
        taskSet.insert(task); 
        }

    void removeTask(int task) { taskSet.erase(task); }
};
