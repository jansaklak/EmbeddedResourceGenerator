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
    bool operator<(const COM& other) const;
    int getSize() const;
    int getID() const;
    int getBandwidth() const;
    int getCost() const;
    bool isConnected(const Hardware* other) const;
    void PrintCOM(int hw_size,std::ostream& out) const;

};


#endif // COM_H
