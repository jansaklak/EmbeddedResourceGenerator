#pragma once
#include <iostream>
#include <set>
#ifndef HARDWARE_H
#define HARDWARE_H

extern const int SCALE;

enum class Hardware_Type {
    HC,
    PE
};

class Hardware {
    private:
        int cost;
        Hardware_Type H_type;
        int restrictions;
        int id;
    public:
        Hardware(double power, Hardware_Type type,int set_id);
        Hardware(int _type,int _cost,int _id);
        bool operator<(const Hardware& other) const;
        int getID();
        int getCost();
        void PrintHW(std::ostream& out = std::cout);
};
#endif // HARDWARE_H
