#include "Hardware.h"
#include <unordered_set>
#include <iostream>
#include "Times.h"
class Instance {
private:
    int id;
    const Hardware* hardwarePtr;
    std::set<int> taskSet;
    bool is_Virtual;

public:
    Instance(int _id, const Hardware* _hardwarePtr)
        : id(_id), hardwarePtr(_hardwarePtr),is_Virtual(false){}

    Instance(int _id, const Hardware* _hardwarePtr,bool _isVirtual)
        : id(_id), hardwarePtr(_hardwarePtr),is_Virtual(_isVirtual){}

    int getID() const { 
        return id;
    }
    
    const Hardware* getHardwarePtr() const {return hardwarePtr;}

     bool isVirtual() const{
        return is_Virtual;
    }

    bool operator<(const Instance& other) const {
        if (hardwarePtr != other.hardwarePtr) {
            return hardwarePtr < other.hardwarePtr;
        }
        return id < other.id;
    }
    friend std::ostream& operator<<(std::ostream& os, const Instance& instance) {
        if(instance.isVirtual()){
            os << "V_" << *instance.getHardwarePtr() << "_" << instance.getID();
        }
        else{
            os << *instance.getHardwarePtr() << "_" << instance.getID();
        }
        return os;
    }
    
    const std::set<int>& getTaskSet() const { return taskSet; }

    void addTask(int task) { 
        taskSet.insert(task); 
        }

    void removeTask(int task) { taskSet.erase(task); }
};
