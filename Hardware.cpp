
#include "Hardware.h"
#include <cstdlib>
#include <iostream>

Hardware::Hardware(double power, Hardware_Type type,int _id) {
    if (power > 10) power = 10;
    double mn = std::rand() % (2 * SCALE) + SCALE;
    if (type == Hardware_Type::HC) {
        H_type = Hardware_Type::HC;
    } else {
        H_type = Hardware_Type::PE;
        mn = mn / 10;
    }
    cost = power * mn;
    restrictions = 0;
    id = _id;
}

bool Hardware::operator<(const Hardware& other) const {
        if (H_type != other.H_type) {
            if(H_type == Hardware_Type::PE){
                return 1;
            }
            else{
                return 0;
            }
        }
        return id < other.id;
}

Hardware::Hardware(int _type,int _cost,int _id) {
    if (_type == 0) {
        H_type = Hardware_Type::HC;
    } else {
        H_type = Hardware_Type::PE;
    }
    restrictions = 0;
    cost = _cost;
    id = _id;
}

std::string Hardware::getType() const{
    if(H_type == Hardware_Type::HC){
        return "HC";
    }
    else{
        return "PE";
    }
}

std::ostream& operator<<(std::ostream& os, const Hardware& hw){
    os << hw.getType() << hw.getID();
    return os;
}

int Hardware::getCost() const {
    return cost;
}

int Hardware::getID() const {
    return id;
}

void Hardware::printHW(std::ostream &out) {
    out << cost << " " << restrictions << " ";
    if (H_type == Hardware_Type::PE) {
        out << 1;
    }
    if (H_type == Hardware_Type::HC) {
        out << 0;
    }
}
