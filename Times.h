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
    std::vector<std::vector<int>> normalized_matrix;
    void CountCosts();
    void clear();
public:
    Times();
    Times(int graph_size);
    void setRandomTimesAndCosts();
    void setTimesMatrix(std::vector<std::vector<int>> _times_matrix);
    void setCostsMatrix(std::vector<std::vector<int>> _costs_matrix);
    void show(std::ostream& out = std::cout) const;
    int getTime(int TaskID,const Hardware* h) const;
    int getCost(int TaskID,const Hardware* h) const;
    int getNormalized(int TaskID,const Hardware* h) const;
    void printTimes(std::ostream& out = std::cout) const;
    void printCosts(std::ostream& out = std::cout) const;

    void updateNormalized(int TaskID,const Hardware* h);
    void LoadHW(std::vector<Hardware> _HW_vec);


};

#endif // TIMES_H
