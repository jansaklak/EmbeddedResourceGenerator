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
        Hardware(double adv, Hardware_Type type,int set_id);
        Hardware(int type,int cost_set,int set_id);
        bool operator<(const Hardware& other) const;
        int getID();
        int getCost();
        void PrintHW(std::ostream& out = std::cout);
};

class Channel{
    private:
        int pin_cost;
        int bandwidth;
        std::set<Hardware> connected_hw;
    public:
        Channel(int pincost,int bd);
        void connect(Hardware h);
};

#endif // HARDWARE_H
