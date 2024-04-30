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
        std::vector<int> allocated_tasks;
        std::vector<int> progress;
        std::vector<std::set<int>> HWtoTasks;
        std::vector<Instance*> Instances;
        std::map<int, int> HWInstancesCount;
        std::map<int, Instance*> taskInstanceMap;
        std::map<int,std::pair<int, int>> task_schedule;

        Graf TaskGraph;
        Times times;
        int tasks_amount;
        int hardware_cores_amount;
        int processing_unit_amount;
        int channels_amount;
        bool with_cost;
        int TotalCost;
        int simulation_time_scale;

        //Randomizing
        void createRandomTasksGraph();
        void connectRandomCH();
        int createRandomProc();

        //Simple getters
        Times getTimes() const;
        Graf getGraph() const;
        std::vector<Hardware> getHardwares() const;
        std::vector<COM> getCOMS() const;

        //Adv getters
        int getStartingTime(int task_id);
        int getEndingTime(int task_id);
        int getInstanceStartingTime(const Instance* inst);
        int getInstanceEndingTime(const Instance* inst);
        void getCurrWeight(int task_id,bool changeInstances,int MAX_TIME);
        int getCriticalTime();
        const Instance* getShortestRunningInstance();
        const Instance* getLongestRunningInstance();
        int getTimeRunning(const Instance* inst);
        int getIdleTime(const Instance* inst,int timeStop);
        Hardware* getLowestTimeHardware(int task_id, int time_cost_normalized) const;
        std::vector<int> getLongestPath(int start) const;
        
        //Printing
        void printSchedule();

        //Instances
        Instance* getInstance(int task_id);
        int createInstance(int task_ID,const Hardware* h);
        int createInstance(int task_ID);
        void addTaskToInstance(int task_ID,Instance* inst);
        void removeTaskFromInstance(int task_ID);
        void sortTaskSet(Instance* inst);
        void removeTaskHelper(int task_ID);

        //Simulation
        void TaskRunner(Instance i);
        void ReadProgress();

        //Normalizing
        void recurrent_distribution_helper(int root,std::vector<int> _currSet);
        double time_cost_proc(int task_id,const Instance* inst,double t_factor = 1.0,double c_factor=1.0,double p_factor=1.0);
        double time_cost(int task_id,const Instance* inst);
        double time_weight(int task_id,const Instance* inst);
        double reuse_time_weight(int task_id,const Instance* inst);
        double cost_weight(int task_id,const Instance* inst);
        double allocated_cost(int task_id,const Instance* inst,int MAX_TIME);
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
        void clear();

        //Scheduling
        void taskDistribution(int rule);
        void runTasks();

        //Printing
        void printTasks(std::ostream& out = std::cout) const;
        void printProc(std::ostream& out = std::cout) const;
        void printCOMS(std::ostream& out = std::cout) const;
        void printALL(std::string filename,bool toScreen) const;
        void printToGantt(std::string filename="gantt_data.dat");
        void printInstances();
        //void TaskProgress(int task_id, int time, int hw_id);
};

#endif // COST_LIST_H