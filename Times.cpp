
#include "Hardware.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include "Times.h"

void Times::CountCosts() {
    
}

Times::Times(){
    return;
}
Times::Times(int verticles, std::vector<Hardware> v) {
    graph_size = verticles;
    hw_vec = v;
}

void Times::setTimesMatrix(std::vector<std::vector<int>> times){
    times_matrix = times;
}

void Times::setCostsMatrix(std::vector<std::vector<int>> costs){
    cost_matrix = costs;
}

void Times::SetRandomTimesAndCosts() {
    int times,cost;
    double adv;
    times_matrix.clear();
    cost_matrix.clear();
    std::vector<int> v_times;
    std::vector<int> v_costs;
    for (int t = 0; t < graph_size; t++) {
       
        for (Hardware hw : hw_vec) {
            
            adv = 1 + rand() % (SCALE / 4);
            times = adv * SCALE * sqrt(SCALE) / (hw.getCost() + rand() % (SCALE / 8));
            cost = SCALE * 8 / times + rand() % (SCALE / 8);
            v_times.push_back(times);
            v_costs.push_back(cost);
        }
        times_matrix.push_back(v_times);
        cost_matrix.push_back(v_costs);
        v_times.clear();
        v_costs.clear();
    }
    return;
}

void Times::show(std::ostream& out) {
    printTimes(out);
    printCosts(out);
}

void Times::printTimes(std::ostream& out) {
    out << "@times\n";
    for (std::vector<int> v : times_matrix) {
        for (int i : v) {
            out << i << " ";
            
        }
        out << std::endl;
    }
}

void Times::printCosts(std::ostream& out) {
    out << "@costs\n";
    for (std::vector<int> v : cost_matrix) {
        for (int i : v) {
            out << i << " ";
        }
        out << std::endl;
    }
}
