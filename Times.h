#pragma once

#ifndef TIMES_H
#define TIMES_H

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#include "Graf.h"

class Times {
private:
    int graph_size;
    std::vector<Hardware> HW_vec;
    std::vector<std::vector<int>> times_matrix;
    std::vector<std::vector<int>> cost_matrix;
    void CountCosts();
public:
    Times();
    Times(int graph_size, std::vector<Hardware> _HW_vec);
    void SetRandomTimesAndCosts();
    void setTimesMatrix(std::vector<std::vector<int>> _times_matrix);
    void setCostsMatrix(std::vector<std::vector<int>> _costs_matrix);
    void show(std::ostream& out = std::cout);
    int getTime(int TaskID,int HW_ID);
    int getCost(int TaskID,int HW_ID);
    void printTimes(std::ostream& out = std::cout);
    void printCosts(std::ostream& out = std::cout);
};

#endif // TIMES_H
