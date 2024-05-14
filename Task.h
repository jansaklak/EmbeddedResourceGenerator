#include <iostream>
#include <utility>
#include <vector>


class Task {
private:
    int id;
    int type;
// 0-normal 1-warunkowe
public:
    Task(int _id){
        id = id;
    }
    int getID(){
        return id;
    }
    int getType(){
        return type;
    }

    std::ostream& operator<<(std::ostream& os, const Task& t) {
    os << t.getID();
    return os;
}