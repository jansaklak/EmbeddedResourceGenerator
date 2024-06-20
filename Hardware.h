#pragma once
#include <iostream>
#include <set>
#ifndef HARDWARE_H
#define HARDWARE_H
#include <string>
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
        int getID() const;
        int getCost() const;
        int _getCost() const;
        void printHW(std::ostream& out = std::cout);
        bool operator<(const Hardware& other) const;
        std::string getType() const;
        Hardware(double power, Hardware_Type type,int set_id);
        Hardware(int _type,int _cost,int _id);
        friend std::ostream& operator<<(std::ostream& os, const Hardware& hw);
};
#endif // HARDWARE_H
