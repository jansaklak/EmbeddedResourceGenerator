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
#include <map>
#include "Instance.h"
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
        void TaskRunner(Instance i);
        std::vector<Instance*> Instances;
        std::map<int, int> HWInstancesCount;
        std::map<int, Instance*> taskInstanceMap;
    public:
        
        Cost_List(int _tasks,int _hcores,int _punits,int _channels,int _withCost);
        Cost_List();
        ~Cost_List();
        void CreateRandomTasksGraph();
        void printTasks(std::ostream& out = std::cout) const;
        void PrintProc(std::ostream& out = std::cout) const;
        void PrintCOMS(std::ostream& out = std::cout) const;
        void ConnectRandomCH();
        void getRandomProc();
        void RandALL();
        void RunTasks();
        void ReadProgress();
        void TaskDistribution(int rule);
        int getStartingTime(int task_id);
        int getEndingTime(int task_id);
        void addTaskToInstance(int task_ID,Instance* inst);
        void removeTaskFromInstance(int task_ID);
        void PrintALL(std::string filename,bool toScreen) const;
        Hardware* getLowestTimeHardware(int task_id, int time_cost) const;
        std::vector<int> getLongestPath(int start) const;
        void createInstance(int task_ID,const Hardware* h);
        void createInstance(int task_ID);
        int getInstanceEndingTime(const Instance* inst);
        void Load_From_File(const std::string& filename);
        std::vector<Hardware> getHardwares() const;
        std::vector<COM> getCOMS() const;
        Instance* getInstance(int task_id);
        Graf getGraph() const;
        Times getTimes() const;
        //void TaskProgress(int task_id, int time, int hw_id);
};

#endif // COST_LIST_H
