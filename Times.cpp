
#include "Hardware.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include "Times.h"

Times::Times(){
    return;
}
Times::Times(int _size, std::vector<Hardware> _HW_vec) {
    graph_size = _size;
    HW_vec = _HW_vec;
}

void Times::setTimesMatrix(std::vector<std::vector<int>> _times_matrix){
    times_matrix.clear();
    times_matrix = _times_matrix;
}

void Times::setCostsMatrix(std::vector<std::vector<int>> _cost_matrix){
    cost_matrix.clear();
    cost_matrix = _cost_matrix;
}

void Times::SetRandomTimesAndCosts() {
    int randTime;
    int randCost;
    double randComplexity;
    times_matrix.clear();
    cost_matrix.clear();
    std::vector<int> times_row;
    std::vector<int> costs_row;
    for (int t = 0; t < graph_size; t++) {
        for (Hardware hw : HW_vec) {
            randComplexity = 1 + rand() % (SCALE / 4);
            randTime = randComplexity * SCALE * sqrt(SCALE) / (hw.getCost() + rand() % (SCALE / 8));
            randCost = SCALE * 8 / randTime + rand() % (SCALE / 8);
            times_row.push_back(randTime);
            costs_row.push_back(randCost);
        }
        times_matrix.push_back(times_row);
        cost_matrix.push_back(costs_row);
        times_row.clear();
        costs_row.clear();
    }
    return;
}

void Times::show(std::ostream& out) {
    printTimes(out);
    printCosts(out);
}

void Times::printTimes(std::ostream& out) {
    out << "@times\n";
    for (std::vector<int> row : times_matrix) {
        for (int i : row) {
            out << i << " ";
        }
        out << std::endl;
    }
}

void Times::printCosts(std::ostream& out) {
    out << "@cost\n";
    for (std::vector<int> row : cost_matrix) {
        for (int i : row) {
            out << i << " ";
        }
        out << std::endl;
    }
}


int Times::getTime(int TaskID,int HW_ID){
    std::vector<int> row = times_matrix[TaskID];
    return row[HW_ID];
}

int Times::getCost(int TaskID,int HW_ID){
    std::vector<int> row = cost_matrix[TaskID];
    return row[HW_ID];
}
