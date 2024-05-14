
#include "Hardware.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include "Times.h"
#include <algorithm>


Times::Times(){
    clear();
    return;
}

void Times::clear(){
    HW_vec.clear();
    times_matrix.clear();
    cost_matrix.clear();
    normalized_matrix.clear();
    return;
}

void Times::LoadHW(std::vector<Hardware> _HW_vec){
    HW_vec = _HW_vec;
}

Times::Times(int _size) {
    clear();
    graph_size = _size;
    return;
}

void Times::setTimesMatrix(std::vector<std::vector<int>> _times_matrix){
    times_matrix.clear();
    times_matrix = _times_matrix;
    return;
}

void Times::setCostsMatrix(std::vector<std::vector<int>> _cost_matrix){
    cost_matrix.clear();
    cost_matrix = _cost_matrix;
    return;
}

void Times::normalize(double task_ratio,double cost_ratio,double time_ratio) {
    normalized_matrix.resize(times_matrix.size());
    for(int i = 0; i < times_matrix.size(); i++) {
        normalized_matrix[i].resize(times_matrix.size());
        for(int j = 0; j < times_matrix.size(); j++) {
            normalized_matrix[i][j] = times_matrix[i][j] * cost_matrix[i][j];
        }
    }
}

void Times::setRandomTimesAndCosts() {
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
            randTime = randComplexity * SCALE * sqrt(SCALE) / (hw._getCost() + 1 + rand() % (SCALE / 8));
            randCost = SCALE * 8 / 1 + randTime + rand() % (SCALE / 8);
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

void Times::show(std::ostream& out) const{
    printTimes(out);
    printCosts(out);
    return;
}

void Times::printTimes(std::ostream& out) const{
    out << "@times\n";
    for (std::vector<int> row : times_matrix) {
        for (int i : row) {
            out << i << " ";
        }
        out << std::endl;
    }
    return;
}

void Times::printCosts(std::ostream& out) const{
    out << "@cost\n";
    for (std::vector<int> row : cost_matrix) {
        for (int i : row) {
            out << i << " ";
        }
        out << std::endl;
    }
    return;
}


int Times::getTime(int TaskID, const Hardware* h) const {
    if (h == nullptr) return -1;
    std::vector<int> row = times_matrix[TaskID];
    return row[h->getID()];
}
int Times::getCost(int TaskID, const Hardware* h) const {
    if (h == nullptr) return -1;
    std::vector<int> row = cost_matrix[TaskID];
    return row[h->getID()];
}

int Times::getNormalized(int TaskID, const Hardware* h) const {
    if (h == nullptr) return -1;
    std::vector<int> row = cost_matrix[TaskID];
    return row[h->getID()];
}
