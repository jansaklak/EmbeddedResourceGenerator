
#include "Hardware.h"
#include <cstdlib>
#include <iostream>

Hardware::Hardware(double adv, Hardware_Type type,int set_id) {
    if (adv > 10) adv = 10;
    double mn = std::rand() % (2 * SCALE) + SCALE;
    if (type == Hardware_Type::HC) {
        H_type = Hardware_Type::HC;
    } else {
        H_type = Hardware_Type::PE;
        mn = mn / 10;
    }
    cost = adv * mn;
    restrictions = 0;
    id = set_id;
}

bool Hardware::operator<(const Hardware& other) const {
        return id < other.id;
}

Hardware::Hardware(int type,int cost_set,int set_id) {
    if (type == 0) {
        H_type = Hardware_Type::HC;
    } else {
        H_type = Hardware_Type::PE;
    }
    restrictions = 0;
    cost = cost_set;
    id = set_id;
}

int Hardware::getCost() {
    return cost;
}

int Hardware::getID() {
    return id;
}

void Hardware::PrintHW(std::ostream &out) {
    out << cost << " " << restrictions << " ";
    if (H_type == Hardware_Type::PE) {
        out << 1;
    }
    if (H_type == Hardware_Type::HC) {
        out << 0;
    }
}
