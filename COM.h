#pragma once

#ifndef COM_H
#define COM_H

#include <iostream>
#include <vector>
#include "Hardware.h"

class COM {
private:
    std::vector<Hardware> line;
    int bandwidth;
    int connect_cost;
    int id;
public:
    COM(int bd,int cost,int set_id);
    COM(std::vector<Hardware> h_list, int bd,int cost,int set_id);
    void add_Hardware(Hardware h);
    bool operator<(const COM& other) const;
    int getSize();
    int getID();
    int getB();
    int getC();
    bool isConnected(int find_id);
    void PrintCOM(int hw_size,std::ostream& out);

};


#endif // COM_H
