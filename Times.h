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
    void CountCosts();
    void clear();
    std::vector<Hardware> HW_vec;
    std::vector<std::vector<int>> times_matrix;
    std::vector<std::vector<int>> cost_matrix;
    std::vector<std::vector<double>> normalized_matrix;
public:
    Times();
    Times(int graph_size);
    int getTime(int TaskID,const Hardware* h) const;
    int getCost(int TaskID,const Hardware* h) const;
    int getNormalized(int TaskID,const Hardware* h) const;
    void setRandomTimesAndCosts();
    void setSubTaskTotalTime(int TaskID,int subTotalTime,int subTotalCost);
    void setTimesMatrix(std::vector<std::vector<int>> _times_matrix);
    void setCostsMatrix(std::vector<std::vector<int>> _costs_matrix);
    void show(std::ostream& out = std::cout) const;
    void printTimes(std::ostream& out = std::cout) const;
    void printCosts(std::ostream& out = std::cout) const;
    void normalize(double task_ratio = 1.0,double cost_ratio = 1.0,double time_ratio = 1.0);
    void updateNormalized(int TaskID,const Hardware* h);
    void LoadHW(std::vector<Hardware> _HW_vec);
};

#endif // TIMES_H
