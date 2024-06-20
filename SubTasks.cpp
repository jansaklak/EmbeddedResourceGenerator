#include "SubTasks.h"
#include <iostream>
#include <algorithm>
#define INF 2147483647
#include <unordered_set>
SubTasks::SubTasks(){

}
                   
// Destructor
SubTasks::~SubTasks() {
    subTaskCosts.clear();
}

// Function to set subtask cost
void SubTasks::setSubTaskCost(int taskID, int subTaskID, int hwID, int cost) {
    SubTaskKey key { taskID, subTaskID, hwID };
    //std::cout << "Task" << taskID << "_" << subTaskID << "on HW: " << hwID << "cost: " << cost << std::endl;
    subTaskCosts[key] = cost;
}

void SubTasks::setSubTaskTime(int taskID, int subTaskID, int hwID, int time) {
    SubTaskKey key { taskID, subTaskID, hwID };
    //std::cout << "Task" << taskID << "_" << subTaskID << "on HW: " << hwID << "time: " << time << std::endl;
    subTaskTimes[key] = time;
}

// Function to get subtask cost
int SubTasks::getSubTaskCost(int taskID, int subTaskID, int hwID) {
    SubTaskKey key { taskID, subTaskID, hwID };
    auto it = subTaskCosts.find(key);
    if (it != subTaskCosts.end()) {
        return it->second; // Return the cost if found
    } else {
        return -1; // Return -1 if not found (assuming -1 indicates cost not found)
    }
}

int SubTasks::getSubTaskTime(int taskID, int subTaskID, int hwID) {
    SubTaskKey key { taskID, subTaskID, hwID };
    auto it = subTaskTimes.find(key);
    if (it != subTaskTimes.end()) {
        return it->second; // Return the cost if found
    } else {
        return -1; // Return -1 if not found (assuming -1 indicates cost not found)
    }
}

int SubTasks::getCheapestHW(int taskID, int subTaskID) {
    int minCost = INF; // Initialize with a large value
    int cheapestHW = -1; // Initialize with an invalid value

    // Iterate over subTaskCosts to find the cheapest HW for given taskID and subTaskID
    for (const auto& entry : subTaskCosts) {
        const SubTaskKey& key = entry.first;
        if (key.taskID == taskID && key.subTaskID == subTaskID) {
            int cost = entry.second;
            if (cost < minCost) {
                minCost = cost;
                cheapestHW = key.hwID;
            }
        }
    }

    //std::cout << "Dla zadania" << taskID << "_" << subTaskID << "min cost = " << minCost << std::endl;

    // Return the cheapest HW ID found (or -1 if not found)
    return cheapestHW;
}

int SubTasks::getFastestHW(int taskID, int subTaskID) {
    int minCost = INF; // Initialize with a large value
    int cheapestHW = -1; // Initialize with an invalid value

    // Iterate over subTaskCosts to find the cheapest HW for given taskID and subTaskID
    for (const auto& entry : subTaskTimes) {
        const SubTaskKey& key = entry.first;
        if (key.taskID == taskID && key.subTaskID == subTaskID) {
            int cost = entry.second;
            if (cost < minCost) {
                minCost = cost;
                cheapestHW = key.hwID;
            }
        }
    }

    //std::cout << "Dla zadania" << taskID << "_" << subTaskID << "min time = " << minCost << std::endl;

    // Return the cheapest HW ID found (or -1 if not found)
    return cheapestHW;
}

int SubTasks::getNumSubTasks(int taskID) const {
    std::unordered_set<int> uniqueSubTaskIDs;

    for (const auto& entry : subTaskCosts) {
        const SubTaskKey& key = entry.first;
        if (key.taskID == taskID) {
            uniqueSubTaskIDs.insert(key.subTaskID);
        }
    }

    return uniqueSubTaskIDs.size();
}