#pragma once

#ifndef COM_H
#define COM_H

#include <iostream>
#include <vector>
#include "Hardware.h"

class COM {
private:
    std::vector<Hardware> HW_line;
    int bandwidth;
    int connect_cost;
    int id;
public:
    COM(int _bandwidth,int _cost,int _id);
    COM(std::vector<Hardware> h_list, int _bandwidth,int _cost,int _id);

    void add_Hardware(const Hardware* h);
    void remove_Hardware(const Hardware* h);

    int getSize() const;
    int getID() const;
    int getBandwidth() const;
    int getCost() const;
    
    bool isConnected(const Hardware* other) const;
    void printCOM(std::ostream& out,std::vector<Hardware> hws) const;

    bool operator<(const COM& other) const;
};


#endif // COM_H