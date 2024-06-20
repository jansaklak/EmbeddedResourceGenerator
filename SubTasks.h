#pragma once
#ifndef SUBTASKS_H
#define SUBTASKS_H

#include <unordered_map>
struct SubTaskKey {
    int taskID;
    int subTaskID;
    int hwID;
    bool operator==(const SubTaskKey& other) const {
        return taskID == other.taskID && subTaskID == other.subTaskID && hwID == other.hwID;
    }
};

struct SubTaskKeyHash {
    std::size_t operator()(const SubTaskKey& key) const {
        return std::hash<int>()(key.taskID) ^ std::hash<int>()(key.subTaskID) ^ std::hash<int>()(key.hwID);
    }
};

class SubTasks {
private:
    std::unordered_map<SubTaskKey, int, SubTaskKeyHash> subTaskCosts;
    std::unordered_map<SubTaskKey, int, SubTaskKeyHash> subTaskTimes;

public:
    SubTasks();
    ~SubTasks();
    int getSubTaskCost(int taskID, int subTaskID, int hwID);
    int getSubTaskTime(int taskID, int subTaskID, int hwID);
    int getFastestHW(int taskID, int subTaskID);
    int getCheapestHW(int taskID, int subTaskID);
    int getNumSubTasks(int taskID) const;
    void setSubTaskCost(int taskID, int subTaskID, int hwID, int cost);
    void setSubTaskTime(int taskID, int subTaskID, int hwID, int cost);
};

#endif // SUBTASKS_H
