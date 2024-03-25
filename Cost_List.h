#pragma once
#ifndef COST_LIST_H
#define COST_LIST_H
#include <mutex>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <queue>
#include "COM.h"
#include "Graf.h"
#include "Hardware.h"
#include "Times.h"
#include <stack>
#include "TimeAndCost.h"
#include <functional>
class Cost_List{
    private:
        std::vector<Hardware> Hardwares;
        std::vector<COM> Channels;
        Graf TaskGraph;
        Times times;
        int tasks_amount;
        int hardware_cores_amount;
        int processing_unit_amount;
        int channels_amount;
        bool with_cost;

        int TotalCost;
        std::vector<int> progress;
        std::vector<std::set<int>> HWtoTasks;
        void TaskRunner(int HWID);
    public:
        
        Cost_List(int _tasks,int _hcores,int _punits,int _channels,int _withCost);
        Cost_List();
        ~Cost_List();
        void CreateRandomTasksGraph();
        void printTasks(std::ostream& out = std::cout);
        void PrintProc(std::ostream& out = std::cout);
        void PrintCOMS(std::ostream& out = std::cout);
        void ConnectRandomCH();
        void getRandomProc();
        void RandALL();
        void RunTasks();
        void TaskProgress(int task_id, int time, int hw_id);
        void ReadProgress();
        void TaskDistribution(int rule);
        void PrintALL(std::string filename,bool toScreen);
        
        void Load_From_File(const std::string& filename);
        std::vector<Hardware> getHardwares();
        std::vector<COM> getCOMS();
        Graf getGraph();
        Times getTimes();
};

#endif // COST_LIST_H
