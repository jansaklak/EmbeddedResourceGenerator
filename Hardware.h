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
        Hardware(double power, Hardware_Type type,int set_id);
        Hardware(int _type,int _cost,int _id);
        bool operator<(const Hardware& other) const;
        int getID() const;
        int getCost() const;
        int _getCost() const;
        std::string getType() const;
        friend std::ostream& operator<<(std::ostream& os, const Hardware& hw);
        void printHW(std::ostream& out = std::cout);
};
#endif // HARDWARE_H
