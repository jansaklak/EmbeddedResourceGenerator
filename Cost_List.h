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
#define INF 2147483647
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




        Times getTimes() const;
        Graf getGraph() const;
        std::vector<Hardware> getHardwares() const;
        std::vector<COM> getCOMS() const;
        void printSchedule();

        std::vector<int> progress;
        std::vector<std::set<int>> HWtoTasks;
        std::vector<Instance*> Instances;
        std::map<int, int> HWInstancesCount;
        std::map<int, Instance*> taskInstanceMap;
        std::map<int,std::pair<int, int>> task_schedule;

        void createRandomTasksGraph();
        int getStartingTime(int task_id);
        int getEndingTime(int task_id);
        int getInstanceStartingTime(const Instance* inst);
        int getInstanceEndingTime(const Instance* inst);

        const Instance* getShortestRunningInstance();
        const Instance* getLongestRunningInstance();
        int getTimeRunning(const Instance* inst);
        int getIdleTime(const Instance* inst,int timeStop);

        Hardware* getLowestTimeHardware(int task_id, int time_cost) const;
        std::vector<int> getLongestPath(int start) const;
        Instance* getInstance(int task_id);

        void createInstance(int task_ID,const Hardware* h);
        void createInstance(int task_ID);
        void addTaskToInstance(int task_ID,Instance* inst);
        void removeTaskFromInstance(int task_ID);

        void connectRandomCH();
        

        void TaskRunner(Instance i);

        void updateWeights();

        void ReadProgress();
    public:

        Cost_List(int _tasks,int _hcores,int _punits,int _channels,int _withCost);
        Cost_List();
        ~Cost_List();
        int Load_From_File(const std::string& filename);
        void clear();
        void randALL();
        void taskDistribution(int rule);
        void runTasks();

        int getRandomProc();

        void printTasks(std::ostream& out = std::cout) const;
        void printProc(std::ostream& out = std::cout) const;
        void printCOMS(std::ostream& out = std::cout) const;
        void printALL(std::string filename,bool toScreen) const;
        void printInstances();
        //void TaskProgress(int task_id, int time, int hw_id);
};

#endif // COST_LIST_H