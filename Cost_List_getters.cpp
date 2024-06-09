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

Instance* Cost_List::getInstance(int task_id) const{
    auto it = taskInstanceMap.find(task_id);
    if (it != taskInstanceMap.end()) {
        return it->second;
    } else {
        std::cerr << "blad dystrybutora zadan dla zadania " << task_id <<  std::endl;
        return nullptr;
    }
}

int Cost_List::getStartingTime(int task_id) {
    if(task_id == 0) return 0;
    // std::cout << "Dla zadania " << task_id << "\n";
    int lowestTime = std::numeric_limits<int>::max();
    std::vector<int> bestPath;
    for (std::vector<int> path : TaskGraph.DFS(0, task_id)) {
        int pathTime = 0;
        bool skipPath = false; // Flaga wskazująca, czy należy pominąć bieżącą ścieżkę
        for (int t_id : path) {
            if (t_id == task_id) {
                break; // Kończymy przetwarzanie ścieżki, jeśli dotarliśmy do szukanego zadania
            } else {
                //std::cout << "ODWIEDZAM " << t_id << "\n";
                const Instance* inst = getInstance(t_id);
                if (inst == nullptr) {
                    skipPath = true;
                    //std::cout << " PRZERYWAM T" << task_id << "//";
                    break;
                }
                const Hardware* hardwarePtr = inst->getHardwarePtr();
                pathTime += times.getTime(t_id, hardwarePtr);
                if(pathTime<task_schedule[t_id].second){
                    std::cout <<"zeruje";
                    // pathTime = task_schedule[t_id].second;
                }
            }
        }
        if (skipPath) {
            continue; // Przechodzimy do następnej ścieżki, pomijając aktualną
        }
        if (pathTime < lowestTime) {
            lowestTime = pathTime;
            bestPath = path;
        }
    }
    // if(lowestTime == 107){
    //     std::cout << " Najlepsza sciezka idzie przez: \n";
    //     for(int tid : bestPath){
    //         if(task_id != tid)
    //         std::cout << "task: " << tid << "na: " << *getInstance(tid) << "ktore konczy o \t\t" << getEndingTime(tid) << "\n";

    //     }
    // }
    return lowestTime;
    
}

int Cost_List::getStartingTimeScheduled(int task_id) {
    if(task_id == 0) return 0;
    // std::cout << "Dla zadania " << task_id << "\n";
    int lowestTime = std::numeric_limits<int>::max();
    std::vector<int> bestPath;
    for (std::vector<int> path : TaskGraph.DFS(0, task_id)) {
        int pathTime = 0;
        bool skipPath = false;
        for (int t_id : path) {
            if (t_id == task_id) {
                break;
            } else {
                //std::cout << "ODWIEDZAM " << t_id << "\n";
                const Instance* inst = getInstance(t_id);
                if (inst == nullptr) {
                    skipPath = true;
                    //std::cout << " PRZERYWAM T" << task_id << "//";
                    break;
                }
                const Hardware* hardwarePtr = inst->getHardwarePtr();
                pathTime += times.getTime(t_id, hardwarePtr);
                if(pathTime<task_schedule[t_id].second){
                    pathTime = task_schedule[t_id].second;
                }
            }
        }
        if (skipPath) {
            continue; // Przechodzimy do następnej ścieżki, pomijając aktualną
        }
        if (pathTime < lowestTime) {
            lowestTime = pathTime;
            bestPath = path;
        }
    }
    return lowestTime;
    
}


int Cost_List::getEndingTime(int task_id) {
    int runningTime = times.getTime(task_id,getInstance(task_id)->getHardwarePtr());
    return getStartingTime(task_id) + runningTime;
}

std::vector<int> Cost_List::getLongestPath(int start) const {
        std::vector<std::vector<Edge>> adjList = TaskGraph.getAdjList();
        std::vector<int> dist(TaskGraph.getVerticesSize(), std::numeric_limits<int>::min());
        std::vector<int> inDegree(TaskGraph.getVerticesSize(), 0);
        std::queue<int> q;
        for (const auto& edges : adjList) {
            for (const auto& edge : edges) {
                int v = edge.getV2();
                inDegree[v]++;
            }
        }
        for (int i = 0; i < TaskGraph.getVerticesSize(); ++i) {
            if (inDegree[i] == 0)
                q.push(i);
        }

        std::vector<int> longestPath;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            longestPath.push_back(u);
            for (const auto& edge : adjList[u]) {
                int v = edge.getV2();
                int w = times.getTime(edge.getV2(),getLowestTimeHardware(edge.getV2(),0));
                if (dist[u] + w > dist[v]) {
                    dist[v] = dist[u] + w;
                }
                inDegree[v]--;
                if (inDegree[v] == 0)
                    q.push(v);
            }
        }

        return longestPath;
    }

    void Cost_List::createPaths(std::vector<std::vector<Edge>> adjList) {
        std::vector<bool> visited(TaskGraph.getVerticesSize(), false);
        bool allVisited = false;
        std::deque<std::deque<int>> queue;
        std::deque<int> firstElementInQueue;
        firstElementInQueue.push_back(0);
        queue.push_back(firstElementInQueue);

        while (!allVisited || queue.size() != 0) {
            std::deque<int> firstVectorFromQueue = queue.front();
            queue.pop_front();
            int current = firstVectorFromQueue.back();
            visited[current] = true;
            std::vector<Edge> neighboursOfCurrent(adjList[current]);

            if (neighboursOfCurrent.size() > 0) {
                for (std::vector<Edge>::iterator it = neighboursOfCurrent.begin(); it != neighboursOfCurrent.end(); it++) {
                    std::deque<int> newVector(firstVectorFromQueue);
                    newVector.push_back((*it).getV2());
                    queue.push_back(newVector);
                }
            }
            else {
                paths.push_back(firstVectorFromQueue);
            }
            allVisited = true;
            for (size_t i = 0; i < visited.size(); i++) {
                allVisited = allVisited && visited[i];
            }
        }
    }

    void Cost_List::printPaths() {
        for (auto const& path : paths) {
            for (auto const& n : path) {
                std::cout << n << " ";
            }
            std::cout << std::endl;
        }
    }

    std::deque<int> Cost_List::getMaxPath() const {
        std::deque<int> maxPath;
        int weight = INT16_MAX;

        for (auto const& path : paths) {
            int currentWeight = 0;
            for (auto const& node : path) {
                currentWeight += (times.getTime(node, getInstance(node)->getHardwarePtr()) * 
                    times.getCost(node, getInstance(node)->getHardwarePtr()));
            }
            if (currentWeight < weight) {
                weight = currentWeight;
                maxPath = path;
            }
        }
        return maxPath;
    }

    Hardware* Cost_List::getLowestTimeHardware(int task_id, int time_cost_normalized) const{
    Hardware* outHW = nullptr;
    int min_time = INF;
    for (const Hardware& hw : Hardwares) {
        int time;
        if(time_cost_normalized == 0){
            time = times.getTime(task_id, &hw);
        }
        else if (time_cost_normalized == 1){
            time = times.getCost(task_id, &hw);
        }
        else if(time_cost_normalized == 2){
            time = times.getNormalized(task_id, &hw);
        }
        if (time < min_time) {
            min_time = time;
            outHW = &const_cast<Hardware&>(hw);
        }
    }
    return outHW;
}

    Hardware* Cost_List::getSlowestHardware(int task_id) const {
        Hardware*outHW = nullptr;
        int maxTime = 0;
        for (const Hardware& hw : Hardwares) {
            if (times.getTime(task_id, &hw) > maxTime) {
                maxTime = times.getTime(task_id, &hw);
                outHW = &const_cast<Hardware&>(hw);
            }
        }
        return outHW;
    }

int Cost_List::getCriticalTime() const{
    int maxTime = 0;
    // for(Instance* i : Instances){
    //     if(getInstanceEndingTime(i)>maxTime) maxTime = getInstanceEndingTime(i);
    // }
    for ( const auto &p : task_schedule )
    {
        if(p.second.second > maxTime){
            maxTime = p.second.second;
        }
    }
    return maxTime;
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

int Cost_List::getIdleTime(const Instance* inst,int timeStop){
        int total_time =0;
        for(int i : inst->getTaskSet()){
            if(getStartingTime(i) + (getEndingTime(i) - getStartingTime(i)) >=timeStop){
                break;
            }
            total_time += getEndingTime(i) - getStartingTime(i);
            
        }
        return timeStop - total_time;
}

const Instance* Cost_List::getLongestRunningInstance(){
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