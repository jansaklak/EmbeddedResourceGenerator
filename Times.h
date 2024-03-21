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
    std::vector<Hardware> hw_vec;
    std::vector<std::vector<int>> times_matrix;
    std::vector<std::vector<int>> cost_matrix;
    void CountCosts();
public:
    Times();
    Times(int graph_size, std::vector<Hardware> v);
    void SetRandomTimesAndCosts();
    void setTimesMatrix(std::vector<std::vector<int>> times);
    void setCostsMatrix(std::vector<std::vector<int>> costs);
    void show(std::ostream& out = std::cout);
    void printTimes(std::ostream& out = std::cout);
    void printCosts(std::ostream& out = std::cout);
};

#endif // TIMES_H
