#include "Cost_List.h"
#include "Times.h"
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "TimeAndCost.h"
#include "Edge.h"
#include <mutex>
#include <algorithm>
#include <map>

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
};

void printWeightTable(const WeightTable& wt) {
    std::cout << "INST: " << *wt.inst << "\t";
    std::cout << "TCP: " << wt.TCP << "\t";
    std::cout << "TC: " << wt.TC << "\t";
    std::cout << "Tw: " << wt.Tw << "\t";
    std::cout << "reTw: " << wt.reTw << "\t";
    std::cout << "Cw: " << wt.Cw << "\t";
    std::cout << "reCw: " << wt.reCw << "\t";
    std::cout << "tasksOnInst: " << wt.StartingTime << "\t";
    std::cout << "runTime: " << wt.runTime << "\t";
    std::cout << "reCalc: " << wt.reCalc << "\t";
    std::cout << "idleTime: " << wt.idleTime << "\t";
    std::cout << "asBefore: " << wt.asBefore << "\n";
}


void Cost_List::getCurrWeight(int task_id){
    std::cout<<"\nDLA ZADANIA " << task_id << "\n";
    double remaining_time;
    double longest_running;
    double least_running;
    double least_time_running;
    double previous_weight;
    std::map<Hardware,double> HWtoWeight;
    std::vector<Instance*> possibleInstances;
    possibleInstances = Instances;
    int i = tasks_amount * 10;
    for (const Hardware& hw : Hardwares){
        const Hardware* hw_ptr = &hw;
        Instance* newInst = new Instance(i,hw_ptr,1);
        possibleInstances.push_back(newInst);
        ++i;
    }
    std::vector<WeightTable> weightsTable;
    for(Instance* inst : possibleInstances){
        WeightTable wt = (
            WeightTable{inst,time_cost_proc(task_id,inst),time_cost(task_id,inst),time_weight(task_id,inst),
                    reuse_time_weight(task_id,inst),cost_weight(task_id,inst),allocated_cost(task_id,inst),
                    inst_starting(task_id,inst),inst_time_running(task_id,inst),
                    reCalculate(task_id,inst),longestIdle(task_id,inst),asBefore(task_id,inst)}
        );
        weightsTable.push_back(wt);
        
        printWeightTable(wt);
    }


}

double Cost_List::time_cost_proc(int task_id,const Instance* inst){
    double val;
    val = times.getTime(task_id,inst->getHardwarePtr()) * 1.0 + times.getCost(task_id,inst->getHardwarePtr());
    if(inst->isVirtual()){
         val += (inst->getHardwarePtr()->getCost() * 1.0);
    }
    return val;
}

double Cost_List::time_cost(int task_id,const Instance* inst){
    return times.getTime(task_id,inst->getHardwarePtr()) * 1.0 + times.getCost(task_id,inst->getHardwarePtr());
}

double Cost_List::time_weight(int task_id,const Instance* inst){
    return times.getTime(task_id,inst->getHardwarePtr());
}

double Cost_List::reuse_time_weight(int task_id,const Instance* inst){
    if(inst->isVirtual()) return 0;
    return times.getTime(task_id,inst->getHardwarePtr());    
}

double Cost_List::cost_weight(int task_id,const Instance* inst){
    return times.getCost(task_id,inst->getHardwarePtr());
}

double Cost_List::allocated_cost(int task_id,const Instance* inst){
    return 0;
}

double Cost_List::inst_starting(int task_id,const Instance* inst){
    if(inst->isVirtual()) return -1;
    return inst->getTaskSet().size();
}

double Cost_List::inst_time_running(int task_id,const Instance* inst){
    if(inst->isVirtual()) return -1;
    return getTimeRunning(inst);
}

double Cost_List::reCalculate(int task_id,const Instance* inst){
    return 0;
}

double Cost_List::longestIdle(int task_id,const Instance* inst){
    if(inst->isVirtual()) return -1;
    return getIdleTime(inst,getStartingTime(task_id));
}

double Cost_List::asBefore(int task_id,const Instance* inst){
    if(inst->isVirtual()){
        return -1;
    }
    for(int i : TaskGraph.getInNeighbourIndices(task_id)){
        if(getInstance(i) == inst){
            return 1;
        }
    }
    return 0;
}





int Cost_List::getInstanceStartingTime(const Instance* inst){
    int startingTime= 0;
    for(int i : inst->getTaskSet()){
        if(getStartingTime(i)>startingTime) startingTime = getStartingTime(i);
    }
    return startingTime;
}

int Cost_List::getInstanceEndingTime(const Instance* inst){
    int endingTime= 0;
    for(int i : inst->getTaskSet()){
        if(getEndingTime(i)>endingTime) endingTime = getEndingTime(i);
    }
    return endingTime;
}

int Cost_List::getTimeRunning(const Instance* inst){
    int total_time =0;
    for(int i : inst->getTaskSet()){
        total_time += getEndingTime(i) - getStartingTime(i);
    }
    return total_time;
}

int Cost_List::getIdleTime(const Instance* inst,int timeStop) {
        int total_time =0;
        for(int i : inst->getTaskSet()){
            if(getStartingTime(i)>=timeStop){
                break;
            }
            total_time += getEndingTime(i) - getStartingTime(i);
            
        }
        if(timeStop - total_time < 0) return 0;
        return timeStop - total_time;
}

const Instance* Cost_List::getLongestRunningInstance() {
    int longest_running = std::numeric_limits<int>::min();
    const Instance* longest = nullptr;
    for (const Instance* inst : Instances) {
        int running_time = getTimeRunning(inst);
        if (running_time > longest_running) {
            longest_running = running_time;
            longest = inst;
        }
    }
    return longest;
}

const Instance* Cost_List::getShortestRunningInstance() {
        int shortest_running = std::numeric_limits<int>::max();
        const Instance* shortest = nullptr;
        for (const Instance* inst : Instances) {
        int running_time = getTimeRunning(inst);
            if (running_time < shortest_running) {
                shortest_running = running_time;
                shortest = inst;
            }
        }
        return shortest;
}