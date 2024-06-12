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




std::vector<Hardware> Cost_List::getHardwares() const{
    return Hardwares;
}

std::vector<COM> Cost_List::getCOMS() const{
    return Channels;
}

Graf Cost_List::getGraph() const{
    return TaskGraph;
}
Times Cost_List::getTimes() const{
    return times;
}

Cost_List::~Cost_List() {

}



Cost_List::Cost_List(){ 
    tasks_amount = 0;
    hardware_cores_amount = 0;
    processing_unit_amount = 0;
    channels_amount = 0;
    with_cost = 0;
    clear();
}

Cost_List::Cost_List(int _tasks, int _hcores, int _punits, int _channels, int _withCost)
    : tasks_amount(_tasks), hardware_cores_amount(_hcores),
    processing_unit_amount(_punits), channels_amount(_channels),
    with_cost(_withCost) {
        clear();
        //std::cout << "Tworze tablice kosztow z task_amount:" << tasks_amount << "hc" << hardware_cores_amount << "pu" <<processing_unit_amount << "ch" << channels_amount << "with_cost" << with_cost;
}

void Cost_List::clear(){
    Hardwares.clear();
    Channels.clear();
    times = Times();
    TaskGraph = Graf();
    HWInstancesCount.clear();
    Instances.clear();
    taskInstanceMap.clear();
    HWtoTasks.clear();
    task_schedule.clear();
    allocated_tasks.clear();
    progress.clear();
    unpredictedTasks.clear();
    conditionalTasks.clear();
}


void Cost_List::clearNUM(){
    int tasks_amount = 0;
    int hardware_cores_amount = 0;
    int processing_unit_amount = 0;
    int channels_amount = 0;
    bool with_cost = 0;
    int TotalCost = 0;
    int simulation_time_scale = 0;
}
    



void Cost_List::randALL(){
    times = Times(tasks_amount);
    createRandomTasksGraph();
    createRandomProc();
    times.LoadHW(Hardwares);
    times.setRandomTimesAndCosts();
    connectRandomCH();
}

void Cost_List::printALL(std::string filename,bool toScreen) const{
    std::ofstream outputFile(filename, std::ofstream::trunc);
    if (outputFile.is_open()) {
        // std::ofstream outputFileMatrix("matrix.dat", std::ofstream::trunc);
        // TaskGraph.printMatrix(outputFileMatrix);
    //@tasks
        if(toScreen) printTasks();
        printTasks(outputFile);
    //@proc
        if(toScreen) printProc();
        printProc(outputFile);
    //@times @cost
        if(toScreen) times.show();
        times.show(outputFile);
    //@coms
        printCOMS(outputFile);
        if(toScreen) printCOMS();
        outputFile.close();
        std::cout << "Zapisano liste do pliku " << filename << std::endl;
    }
    else {
        std::cerr << "Nie można otworzyć pliku do zapisu." << std::endl;
        return;
    }
}

int Cost_List::createRandomProc() {
    Hardwares.clear();
    if(hardware_cores_amount < 1 || processing_unit_amount < 1){
        std::cerr << "Błedna liczba HC lub PU\n";
        return -1;
    }
    for(int i = 0; i<hardware_cores_amount;i++){
        Hardwares.push_back(Hardware(getRand(5)+1,Hardware_Type::HC,Hardwares.size()));
    }
    int HC_size = hardware_cores_amount;
    for(int j = 0; j<processing_unit_amount;j++){
        Hardwares.push_back(Hardware(getRand(5)+1,Hardware_Type::PE,Hardwares.size()-HC_size));
    }
    return 1;
}

void Cost_List::connectRandomCH(){
    if(channels_amount < 1){
        std::cerr<< "Zła liczba kanałów";
        return;
    }
    int bd = ((getRand(9)+1)*10);
    int con_cost = (getRand(9)+1)*5;
    int rd;
    COM c = COM(bd,con_cost,Channels.size());
    
    // for(Hardware h : Hardwares){
    //         std::cout << "DODAJE" << h;
    //         c.add_Hardware(&h);
    // }
    Channels.push_back(c);
    if(channels_amount>1){
        for(int j =1;j<channels_amount;j++){
            bd = ((getRand(9)+1)*10);
            con_cost = (getRand(9)+1)*5;
            COM c = COM(bd,con_cost,Channels.size());
            for(int k = 0;k<(Hardwares.size()/2);k++){
                if(randomBool(3)){
                    Hardware randHW = Hardwares[getRand(Hardwares.size() - 1)];
                    Channels[0].remove_Hardware(&randHW);
                    c.add_Hardware(&randHW);
                }
            }
            Channels.push_back(c);
        }
    }
    for(COM c : Channels){
        if(c.getSize()<1){
            Hardware randHW = Hardwares[getRand(Hardwares.size() - 1)];
            while(1){
                randHW = Hardwares[getRand(Hardwares.size() - 1)];
                if(!c.isConnected(&randHW)) break;
            }
            c.add_Hardware(&randHW);
            while(1){
                randHW = Hardwares[getRand(Hardwares.size() - 1)];
                if(!c.isConnected(&randHW)) break;
            }
            c.add_Hardware(&randHW);
        }
    }

    return;
}

void Cost_List::runTasks() {
    int TotalCost = 0;
    simulation_time_scale = 1;
    std::cout << "\nUruchamiam zadania w skali x" << simulation_time_scale << ":\n";
    progress.resize(TaskGraph.getVerticesSize(), -2); // -2 - cant be done flag
    progress[0] = -1;
    std::vector<std::thread> threads;
    int numThreads = Instances.size();
    bool stop = false;
    int czas = 0;
    std::thread counterThread(Licznik, std::ref(stop), std::ref(czas));
    for (Instance* i : Instances) threads.push_back(std::thread(&Cost_List::TaskRunner, this, *i));
    for (std::thread& thread : threads) thread.join();
    stop = true;
    counterThread.join();
    threads.clear();
    std::cout << "\n\nCzas trwania programu: " << czas << " milisekund. (skala x" << simulation_time_scale << ") \n\n";
}


void Cost_List::unpredictedHandler(int task_ID){
    if (allocated_tasks[task_ID] == 1) {
                return;
    }
    std::cout <<" NIe przewidziane jest: " << task_ID << "\n";
    int min_time = INF;
    int time;
    Instance* bestFoundInst = nullptr;
    for (Instance* inst : Instances) {  //Znajdz najtanszy z obecnych Inst
        if(inst->getHardwarePtr()->getType()=="PE"){
            time = times.getTime(task_ID, inst->getHardwarePtr());
            if (time < min_time) {
                min_time = time;
                bestFoundInst = inst;
                std::cout <<" ZNalazlem : " << *bestFoundInst << "\n";
            }
        }
    }
    if (bestFoundInst == nullptr){
        Hardware* bestPE = nullptr;
        int maxTime = 0;
        for (const Hardware& hw : Hardwares) {
            if (hw.getType()=="PE" && times.getTime(task_ID, &hw) > maxTime) {
                maxTime = times.getTime(task_ID, &hw);
                bestPE = &const_cast<Hardware&>(hw);
            }
        }
        bestFoundInst = new Instance(HWInstancesCount[bestPE->getID()], bestPE);
        HWInstancesCount[bestPE->getID()]++;
        Instances.push_back(bestFoundInst);
    }
    bestFoundInst->addTask(task_ID);
    taskInstanceMap[task_ID] = bestFoundInst;
    allocated_tasks[task_ID] = 1;
}



int Cost_List::createInstance(int task_ID, const Hardware* h) {
    
    if(allocated_tasks[task_ID]==1){
        return 0;
    }
    if(unpredictedTasks.find(task_ID) != unpredictedTasks.end()){
        unpredictedHandler(task_ID);
        return -2;
    }
    Instance* newInst = new Instance(HWInstancesCount[h->getID()], h);
    HWInstancesCount[h->getID()]++;
    newInst->addTask(task_ID);
    Instances.push_back(newInst);
    taskInstanceMap[task_ID] = newInst;
    allocated_tasks[task_ID] = 1;
    //std::cout << "\n\n\ntask instnace map dla " << task_ID << " :: " << taskInstanceMap[task_ID] << "\n\n\n";
    return 1;
}

int Cost_List::createInstance(int task_ID) {
    if (allocated_tasks[task_ID] == 1) {
                return 0;
    }
    if(unpredictedTasks.find(task_ID) != unpredictedTasks.end()){
        unpredictedHandler(task_ID);
        return -2;
    }
    Hardware* h = getLowestTimeHardware(task_ID, 0);
    Instance* newInst = new Instance(HWInstancesCount[h->getID()], h);
    HWInstancesCount[h->getID()]++;
    newInst->addTask(task_ID);
    Instances.push_back(newInst);
    taskInstanceMap[task_ID] = newInst;
    
    allocated_tasks[task_ID] = 1;
    return 1;
}

void Cost_List::addTaskToInstance(int task_ID, Instance* inst) {
    if(unpredictedTasks.find(task_ID) != unpredictedTasks.end()){
        unpredictedHandler(task_ID);
        return;
    }
    if(getInstance(task_ID) != nullptr){
        removeTaskFromInstance(task_ID);
    }
    int startingTimeNewTask = getStartingTime(task_ID);
    if(startingTimeNewTask<getInstanceStartingTime(inst)){
        std::set<int> new_set;
        new_set.insert(task_ID);
        for(int tid : inst->getTaskSet()){
            new_set.insert(tid);
        }
        inst->setTasksSet(new_set);
    }
    else{
        inst->addTask(task_ID);
    }
    taskInstanceMap.erase(task_ID);

    taskInstanceMap[task_ID] = inst;
    allocated_tasks[task_ID] = 1;
    return;

    // int startingTimeNewTask = getStartingTime(task_ID);

    // // Temporary set to store tasks in sorted order
    // std::set<int> sortedTaskSet;

    // // Flag to check if the new task has been inserted
    // bool taskInserted = false;
    // for (int taskId : inst->getTaskSet()) {
    //     int startingTimeCurrTask = getStartingTime(taskId);
    //     if (startingTimeNewTask < startingTimeCurrTask && !taskInserted) {
    //         sortedTaskSet.insert(task_ID);
    //         taskInserted = true;
    //     }
    //     sortedTaskSet.insert(taskId);
    // }
    // if (!taskInserted) {
    //     sortedTaskSet.insert(task_ID);
    // }
    // inst->setTasksSet(sortedTaskSet);
    // taskInstanceMap[task_ID] = inst;

    
}


void Cost_List::removeTaskFromInstance(int task_ID){
    if(unpredictedTasks.find(task_ID) != unpredictedTasks.end()){
        unpredictedHandler(task_ID);
        return;
    }
    taskInstanceMap[task_ID]->removeTask(task_ID);
    if(taskInstanceMap[task_ID]->getTaskSet().empty()){
        Instances.erase(std::remove(Instances.begin(), Instances.end(), taskInstanceMap[task_ID]), Instances.end());
    }
    allocated_tasks[task_ID] = 0;
    taskInstanceMap.erase(task_ID);
    return;
}

void Cost_List::TaskRunner(Instance i) {
    int taskCounter = 0;
    const Hardware* running_hw = i.getHardwarePtr();
    while (true) {
        int alltasks = i.getTaskSet().size();
        for (int t : i.getTaskSet()) {
            if (progress[t] == -1) {
                if(conditionalTasks.find(t) != conditionalTasks.end() && evaluateCondition(t) == 1){
                        std::cout << "CT" << t << "skipped";
                        progress[t] = 1;
                }
                else{
                    int time = times.getTime(t, running_hw);
                    {
                        std::cout << "T"<< t << " on " << i << std::endl;
                        progress[t] = 0;
                        for (int i = 1; i < time+1; ++i) {
                            progress[t] = (((i + 1) * 100) / time);
                            std::this_thread::sleep_for(std::chrono::milliseconds(simulation_time_scale));
                        }
                        ++taskCounter;
                        std::cout << "\tT" << t << "done\n";
                    }
                }
                for (int i : TaskGraph.getOutNeighbourIndices(t)) {
                            if(progress[i]==-2) progress[i] = -1;
                    }
                
            }
        }
        if (taskCounter == alltasks) {
            std::cout << "\t\t\t" << i << " ended\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return;
}



//     // std::cout << "Porządek topologiczny: * - stan końcowy, () - czas na najszybszym HW\n";
//     // for(int i : getLongestPath(0)){
//     //     std::cout << i <<"(" << times.getTime(i,getLowestTimeHardware(i,0)) <<")";
//     //     if(TaskGraph.getOutNeighbourIndices(i).size()==0){
//     //         std::cout <<"*";
//     //     }
//     //     std::cout <<"-";
//     // }
//     std::cout << "\n";
//     return;
// }





// std::mutex removeMutex;
// std::mutex addMutex;
// std::condition_variable taskCV;

// std::mutex tasksCounterMutex;

// void Cost_List::TaskProgress(int task_id, int time, int hw_id) {

//     this->progress[task_id] = 1;

//     for (int i = 0; i < time; ++i) {
//         progress[task_id] = ((i + 1) * 100) / time;
//         std::this_thread::sleep_for(std::chrono::milliseconds(1));
//     }

//     addMutex.lock();
//     for(int i : TaskGraph.getOutNeighbourIndices(task_id)){
//         if(!progress[i]>0){
//             tasksToAdd.push_back(i);
//         }
//     }
//     addMutex.unlock();

//     removeMutex.lock();
//     tasksToRemove.push_back(task_id);
//     removeMutex.unlock();


//     HWinUse[hw_id] = 0;
// }



// void Cost_List::ReadProgress() {
//     while(true){
//         for(int i : progress){
//         std::cout << "Task" << i << " ON " << progress[i] << "%\n";
//         }
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }

// }


bool Cost_List::evaluateCondition(int task_id) {
    std::string variable = conditionTaskMap[task_id].variable;
    std::string op = conditionTaskMap[task_id].op;
    int value_int = conditionTaskMap[task_id].value;
    int actual_value_int;
    if(variable == "CRITICAL_TIME"){
        actual_value_int = getCriticalTime();
    }
    else if (variable == "TOTAL_COST")
    {
        actual_value_int = totalCost;
    }
    else if (variable == "HC_INSTANCES")
    {
        actual_value_int = hardware_cores_amount;
    }
    else if (variable == "PE_INSTANCES")
    {
        actual_value_int = processing_unit_amount;
    }
    else if (variable == "TOTAL_INSTANCES")
    {
        actual_value_int = Hardwares.size();
    }
    else if (variable == "CURR_TIME"){
        actual_value_int = getStartingTime(task_id);
    }
    else{
        if (CostListConfig.find(variable) == CostListConfig.end()) {
            std::cerr << "Error: Variable " << variable << " not found in config." << std::endl;
            return false;
        }
        std::string actual_value = CostListConfig.at(variable);
        actual_value_int = std::stoi(actual_value);
    }


    
    

    // Compare the actual value with the provided value based on the operator
    if (op == ">" && actual_value_int > value_int) {
        return true;
    } else if (op == ">=" && actual_value_int >= value_int) {
        return true;
    } else if (op == "<" && actual_value_int < value_int) {
        return true;
    } else if (op == "<=" && actual_value_int <= value_int) {
        return true;
    } else if (op == "==" && actual_value_int == value_int) {
        return true;
    }

    return false;
}

