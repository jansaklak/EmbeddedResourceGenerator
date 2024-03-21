#pragma once
#ifndef COST_LIST_H
#define COST_LIST_H

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include "COM.h"
#include "Graf.h"
#include "Hardware.h"
#include "Times.h"

class Cost_List{
    private:
        Graf TaskGraph;
        std::vector<Hardware> HW_vec;
        std::vector<COM> ch_vec;
        Times t;
        int tasks_amount;
        int hc_amount;
        int pu_amount;
        int ch_amount;
        bool with_cost;
        bool toScreen;
    public:
        Cost_List(int tasks,int hcores,int pus,int channels,int withcost,int toscreen);
        Cost_List();
        void CreateRandomTasksGraph();
        void printTasks(std::ostream& out = std::cout);
        void PrintProc(std::ostream& out = std::cout);
        void PrintCOMS(std::ostream& out = std::cout);
        void ConnectRandomCH();
        void getRandomProc();
        void RandALL();
        void PrintALL(std::string filename);
        void Load_From_File(const std::string& filename);
};

#endif // COST_LIST_H
