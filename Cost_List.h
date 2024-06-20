#pragma once
#ifndef COST_LIST_H
#define COST_LIST_H

#include <mutex>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
#include <map>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <iomanip>
#include <algorithm>

#include "Instance.h"
#include "COM.h"
#include "Graf.h"
#include "Hardware.h"
#include "Times.h"
#include "Edge.h"
#include "TimeAndCost.h"
#include "SubTasks.h"

#define INF 2147483647

#include <unordered_map>

struct TaskData {
    std::string variable;
    std::string op;
    int value;
};

struct WeightTable {
    Instance* inst;
    double TCP;
    double TC;
    double Tw;
    double reTw;
    double Cw;
    double reCw;
    double StartingTime;
    double runTime;
    double reCalc;
    double idleTime;
    double asBefore;
    double SUM;
};

struct CompareData {
    std::string variable;
    std::string op;
    int value;
};

static int getRand(int MAX){
    if(MAX < 1) MAX = 1;
    return rand() % MAX;
}

static bool randomBool(int prob) {
    prob++;
    return rand() % prob == 0;
}

static void Licznik(bool& stop, int& czas) {
    auto start = std::chrono::steady_clock::now();
    while (!stop) {
        auto current = std::chrono::steady_clock::now();
        czas = std::chrono::duration_cast<std::chrono::milliseconds>(current - start).count();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

class Cost_List{
    private:
    //Variables
        bool with_cost;
        int totalCost;
        int tasks_amount;
        int hardware_cores_amount;
        int processing_unit_amount;
        int channels_amount;
        int TotalCost;
        int simulation_time_scale;
    //Vectors
        std::vector<Hardware> Hardwares;
        std::vector<COM> Channels;
        std::vector<int> allocated_tasks;
        std::vector<int> progress;
        std::vector<std::set<int>> HWtoTasks;
        std::vector<Instance*> Instances;
        std::vector<std::deque<int>> paths;
        std::vector<int> findAllToSkipAfterConditional(int taskID);
    //Maps
        std::map<int, int> HWInstancesCount;
        std::map<int, Instance*> taskInstanceMap;
        std::map<int,std::pair<int, int>> task_schedule;
        std::map<int, std::string> conditions;
        std::map<int, TaskData> conditionTaskMap;
        std::map<int,std::vector<Hardware>> subTaskHW;
        std::unordered_map<std::string, std::string> CostListConfig;
    //Sets
        std::set<int> unpredictedTasks;
        std::set<int> conditionalTasks;
        std::set<int> extendedTasks;
    //Classes
        SubTasks sumTasksTable;
        Graf TaskGraph;
        Times times;
    //Randomizing
        void createRandomTasksGraph();
        void connectRandomCH();
        int createRandomProc();
    //Loading
        int Load_Config();
    //Simple getters
        Times getTimes() const;
        Graf getGraph() const;
    //Advanced getters
        void getCurrWeight(int task_id,bool changeInstances,int MAX_TIME);
        int getStartingTime(int task_id);
        int getStartingTimeScheduled(int task_id);
        int getEndingTime(int task_id);
        int getInstanceStartingTime(const Instance* inst);
        int getInstanceEndingTime(const Instance* inst);
        int getCriticalTime() const;
        int getTimeRunning(const Instance* inst) ;
        int getIdleTime(const Instance* inst,int timeStop) ;
        std::vector<int> getLongestPath(int start) const;
        std::vector<Hardware> getHardwares() const;
        std::vector<COM> getCOMS() const;
        std::deque<int> getMaxPath(std::vector<int> toSkip) const;
        Hardware* getLowestTimeHardware(int task_id, int time_cost_normalized) const;
        Hardware* getSlowestHardware(int) const;
        const Instance* getShortestRunningInstance() ;
        const Instance* getLongestRunningInstance() ;
    //Setting instances
        Instance* getInstance(int task_id) const;
        int createInstance(int task_ID,const Hardware* h);
        int createInstance(int task_ID);
        void addTaskToInstance(int task_ID,Instance* inst);
        void removeTaskFromInstance(int task_ID);
        void sortTaskSet(Instance* inst);
        void removeTaskHelper(int task_ID);
        void calculateTotalCost();
    //Handling
        void subTaskHandler(int task_id);
        void unpredictedHandler(int task_ID);
        int evaluateCondition(int task_id);
        void constructByWeight(std::vector<int> bfs_tasks,int MAX_TIME=INF);
        void printUnpredictedTasks();
        void skipConditional();
    //Paths
        void createPaths(std::vector<std::vector<Edge>>);
        void printPaths();
    //Printing
        void printSchedule();
    //Simulation
        void TaskRunner(Instance i);
        void ReadProgress();
    //Normalizing
        void recurrent_distribution_helper(int root,std::vector<int> _currSet);
        void reallocateFastest(int maxTIME,std::vector<bool>& checked);
        double time_cost_proc(int task_id,const Instance* inst,double t_factor = 1.0,double c_factor=1.0,double p_factor=1.0);
        double time_cost(int task_id,const Instance* inst);
        double time_weight(int task_id,const Instance* inst);
        double reuse_time_weight(int task_id,const Instance* inst);
        double cost_weight(int task_id,const Instance* inst);
        double allocated_cost(int task_id,const Instance* inst,double MAX_TIME);
        double inst_starting(int task_id,const Instance* inst);
        double inst_time_running(int task_id,const Instance* inst);
        double reCalculate(int task_id,const Instance* inst);
        double longestIdle(int task_id,const Instance* inst);
        double asBefore(int task_id,const Instance* inst);
    public:
    //Creating and loading
        Cost_List(int _tasks,int _hcores,int _punits,int _channels,int _withCost);
        Cost_List();
        ~Cost_List();
        int Load_From_File(const std::string& filename);
        void randALL();
        void createRandomConditionalTasksGraph();
        void clear();
        void clearNUM();
        void makeConditional(int Task_ID);
    //Scheduling
        void taskDistribution(int rule);
        void runTasks();
    //Printing
        void printTasks(std::ostream& out = std::cout) const;
        void printProc(std::ostream& out = std::cout) const;
        void printCOMS(std::ostream& out = std::cout) const;
        void printALL(std::string filename,bool toScreen) const;
        void printConditions(std::ostream& out = std::cout) const;
        void printToGantt(std::string filename="gantt_data.dat");
        void printInstances() ;
        void printTotalCost();
        //void TaskProgress(int task_id, int time, int hw_id);
};

#endif // COST_LIST_H