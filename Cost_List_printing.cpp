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


void Cost_List::printSchedule() {
    int instance_ending_time;
    int criticalTime = 0;
    task_schedule.clear();
    for(Instance* i : Instances) {
        instance_ending_time = 0;
        
        //std::cout << "Dla instancji " << *i;
        std::vector<std::pair<int, int>> taskWithStartingTime;
        for (int taskID : i->getTaskSet()) {
            taskWithStartingTime.push_back(std::make_pair(taskID, getStartingTime(taskID)));
        }
        std::sort(taskWithStartingTime.begin(), taskWithStartingTime.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
        for (const auto& pair : taskWithStartingTime) {
            if(instance_ending_time<=pair.second){
                
                //std::cout << "Zadanie " << pair.first << " moge zaczac o " << getStartingTime(pair.first) << "a instancja jest wolna od " << instance_ending_time << "\n" ;
                std::pair<int,int> timeRunning = {getStartingTime(pair.first),getStartingTime(pair.first)+times.getTime(pair.first,i->getHardwarePtr())};
                task_schedule.insert({pair.first,timeRunning});
                instance_ending_time = task_schedule[pair.first].second;
            }
            else{
                
                

                std::pair<int,int> timeRunning = {instance_ending_time,instance_ending_time+times.getTime(pair.first,i->getHardwarePtr())};
                task_schedule.insert({pair.first,timeRunning});
                instance_ending_time = task_schedule[pair.first].second;
            }
        }
    }

}



void printWeightTable(const WeightTable& wt) {
    std::cout << "INST: " << std::setw(1) << *wt.inst << "\t|";
    std::cout << "TCP: " << std::setw(5) << wt.TCP << "|";
    std::cout << "TC: " << std::setw(5) << wt.TC << "|";
    std::cout << "Tw: " << std::setw(5) << wt.Tw << "|";
    std::cout << "reTw: " << std::setw(5) << wt.reTw << "|";
    std::cout << "Cw: " << std::setw(5) << wt.Cw << "|";
    std::cout << "reCw: " << std::setw(5) << wt.reCw << "|";
    std::cout << "tasksOnInst: " << std::setw(5) << wt.StartingTime << "|";
    std::cout << "runTime: " << std::setw(5) << wt.runTime << "|";
    std::cout << "reCalc: " << std::setw(5) << wt.reCalc << "|";
    std::cout << "idleTime: " << std::setw(5) << wt.idleTime << "|";
    std::cout << "asBefore: " << std::setw(5) << wt.asBefore << "\n";
}

void Cost_List::printTasks(std::ostream& out) const{
    out << "@tasks " << tasks_amount << std::endl;
    std::vector<int> outIdx;
    int size = tasks_amount;
    for(int i = 0; i < size; i++) {
        outIdx = TaskGraph.getOutNeighbourIndices(i);
        out << "T" << i << " ";
        out << outIdx.size() << " ";
        for(int j : outIdx) {
            out << j << "(" << TaskGraph.getWeightEdge(i, j) << ") ";
        }
        out << std::endl;
    }
}

void Cost_List::printCOMS(std::ostream& out) const{
    out << "@comm " << Channels.size() <<"\n";
    for(COM c : Channels){
        c.printCOM(out,Hardwares);
    }
}

void Cost_List::printProc(std::ostream& out) const{
    out << "@proc " << Hardwares.size() << std::endl;
    for (Hardware h : Hardwares) {
        h.printHW(out);
         out << std::endl;
    }
}



void Cost_List::printToGantt(std::string filename){
    std::ofstream outputFile(filename, std::ofstream::trunc);
    if (outputFile.is_open()) {
        for(int t = 0; t<tasks_amount;t++){
            outputFile  << t << " " << *getInstance(t) << " " << task_schedule[t].first <<" " << task_schedule[t].second << std::endl;
        }
        std::cout << "Zapisano wykres zadań do: " << filename << std::endl;
        outputFile.close();
    }
    else {
        std::cerr << "Nie można otworzyć pliku do zapisu wykresu." << std::endl;
        return;
    }

}

void Cost_List::printInstances() {
    std::sort(Instances.begin(), Instances.end());
    std::cout << "Stworzono " << Instances.size() << " komponentów\n";
    int task_id;
    int task_time;
    int totalCostOfInstances = 0;
    
    for(int t = 0; t<tasks_amount;t++){
        std::cout  << "T" << t << "\tna " << *getInstance(t) << " od:" << task_schedule[t].first <<" do:" << task_schedule[t].second << std::endl;
    }

    int criticTime = getCriticalTime();
    
    for (const Instance* inst : Instances) {
        int expTime = 0;
        int expCost = 0;
        for (int taskID : inst->getTaskSet()) {
            expTime += times.getTime(taskID, inst->getHardwarePtr());
            expCost += times.getCost(taskID, inst->getHardwarePtr());
        }
        
        expCost += inst->getHardwarePtr()->getCost();
        totalCostOfInstances += expCost;
        std::cout << *inst << " Zadan: " << inst->getTaskSet().size() << " Spodziewany czas: " << expTime << " Bezczynnosci: " << getIdleTime(inst,criticTime) << " koszt: " << expCost  << " w tym początkowy: " << inst->getHardwarePtr()->getCost() << "\n";
    }
    
    std::cout << "Czas scieżki krytycznej: " << criticTime;
    std::cout << "\tKoszt całkowity instancji: " << totalCostOfInstances << std::endl;
}

void Cost_List::printTotalCost() {
    std::cout << "Całkowity koszt systemu z uwzględnieniem kary: " << totalCost << std::endl;
}