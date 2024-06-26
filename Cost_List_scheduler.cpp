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

void Cost_List::recurrent_distribution_helper(int root,std::vector<int> _currSet){
    std::vector<int> currSet = _currSet;
    bool rootInstanceUsed = false;
    if(currSet.empty()){
        return;
    }
    for(int currTask : currSet){
        Hardware* currTask_lowestTimeHW = getLowestTimeHardware(currTask,0);
        if(!rootInstanceUsed && getInstance(root)->getHardwarePtr()==currTask_lowestTimeHW){
            addTaskToInstance(currTask,getInstance(root));
            rootInstanceUsed = true;
        }
        else{
            createInstance(currTask);
        }
    }
    return;
}

//uruchom dla 0
//uruchom dla sasiadow0
//uruchom dla sasiadow z wszystkich sasiadow 0


//PROJEKT
        /*Algorytm rafinacyjny - funckja kary

        1. Zaczynamy od najszybszego podziału zadań

        2. Wybieramy węzeł ( z najlepszą normalizacją) ??

        3. Normalizacja uwzględnia MAX_TIME

        4. Funckja kary - do kosztu dodajemy kare

        5. Koniec: każde z zadań jest rozpatrzone (LOOP_COUNTER) lub nie da się nic zmienić

*/

void Cost_List::taskDistribution(int rule) {
    int estimatedCost = 0;
    int estimatedTime = 0;
    TotalCost = 0;
    int tasks_amount = TaskGraph.getVerticesSize();
    times.normalize();
    int num_allocated = 0;
    allocated_tasks.resize(tasks_amount,0);
    //std::cout << "TASKS AMOUNT: " << tasks_amount;
    switch (rule) {


        case 100:{
            std::vector<int> bfs_tasks = TaskGraph.BFS();
            //std::cout << "Instances.size() = " << Instances.size() << std::endl;
            
            for(int currTask : bfs_tasks){
                Hardware* hw = getLowestTimeHardware(currTask,0);
                for(Instance* inst : Instances){
                    if(inst->getHardwarePtr()==hw && getInstanceEndingTime(inst) <= getStartingTime(currTask)){
                        addTaskToInstance(currTask,inst);
                        allocated_tasks[currTask] = 1;
                        break;
                    }
                }
                createInstance(currTask); // == createInstance(currTask,getLowestTimeHardware(currTask,0));

            }
            break;
        }


        case 8:{        
            int LOOP_COUNTER = 3;
            int HARD_TIME = 250;
            int currTask = 0;
            int PUNISHMENT = 2;
            //int min_time;

            std::vector<int> bfs_tasks = TaskGraph.BFS();
            std::vector<int> tasks_visited_count;
            tasks_visited_count.resize(TaskGraph.getVerticesSize(),LOOP_COUNTER);
            
            //Tworzenie najszybszego podziału razem z ponownym korzystaniem ze sprzetu
            for(int currTask : bfs_tasks){
                Hardware* hw = getLowestTimeHardware(currTask,0);
                for(Instance* inst : Instances){
                    if(inst->getHardwarePtr()==hw && getInstanceEndingTime(inst) <= getStartingTime(currTask)){
                        addTaskToInstance(currTask,inst);
                        allocated_tasks[currTask] = 1;
                        break;
                    }
                }
                createInstance(currTask); // == createInstance(currTask,getLowestTimeHardware(currTask,0));
            }

            //printSchedule();
            //printInstances();
            std::cout << "========================\n\n";
            int possibleMoves = 0;
            for(int t=0;t<TaskGraph.getVerticesSize();t++){
                possibleMoves += tasks_visited_count[t];
            }

            std::deque<int> criticalTimeResults;
            for(int k=0;k<TaskGraph.getVerticesSize();k++){
                criticalTimeResults.push_back(INF - k);
            }

            while(possibleMoves > 0 || criticalTimeResults.front() != criticalTimeResults.back()){ //brak możliwych ruchów albo nie ma poprawy wyników
                //std::cout <<"Molziwe ruchy: " << possibleMoves << "\n";
                createPaths(TaskGraph.getAdjList());
                std::deque<int> maxWezel = getMaxPath(tasks_visited_count);
                
                if(maxWezel.size() == 0) break;
                int Task_to_Check = maxWezel.back();
                

                if(Task_to_Check == 0 || tasks_visited_count[Task_to_Check]>0){ //rozpatruje LOOP_COUNTER razy
                        tasks_visited_count[Task_to_Check]--;
                        int min_time = INF;
                        int cost;
                        Instance* bestFoundInst = nullptr;

                        for (Instance* inst : Instances) {  //Znajdz najtanszy z obecnych Inst
                            cost = times.getCost(Task_to_Check, inst->getHardwarePtr());
                            if (cost < min_time) {
                                min_time = cost;
                                bestFoundInst = inst;
                            }
                        }
                        
                        addTaskToInstance(Task_to_Check,bestFoundInst); //Dodaj do najlepszego (usuwanie automatycznie)
                    }

                printSchedule();

                int totalTime = getCriticalTime();
                if (totalTime > HARD_TIME) {
                    totalCost = 0;
                    for (Instance* instance : Instances) {
                        totalCost += instance->getHardwarePtr()->getCost();
                        for (int taskID : instance->getTaskSet()) {
                            totalCost += times.getCost(taskID, instance->getHardwarePtr());
                        }
                    }
                    totalCost += (totalTime - HARD_TIME) * PUNISHMENT;
                }

                //printInstances();
                
                criticalTimeResults.pop_front();
                criticalTimeResults.push_back(getCriticalTime());
                int possibleMoves = 0;
                for(int t=0;t<TaskGraph.getVerticesSize();t++){
                    possibleMoves += tasks_visited_count[t];
                }
            }
            printTotalCost();
            break;   
        }

        case 81:{        
            int LOOP_COUNTER;
            int HARD_TIME = 10000;
            int currTask = 0;
            int min_time;
            std::vector<int> bfs_tasks = TaskGraph.BFS();
            for(int currTask : bfs_tasks){
                Hardware* hw = getLowestTimeHardware(currTask,0);
                for(Instance* inst : Instances){
                    if(inst->getHardwarePtr()==hw && getInstanceEndingTime(inst) <= getStartingTime(currTask)){
                        addTaskToInstance(currTask,inst);
                        allocated_tasks[currTask] = 1;
                        break;
                    }
                }
                createInstance(currTask);
            }
            for(int currTask : bfs_tasks){
                getCurrWeight(currTask,1,HARD_TIME);
            }

            std::vector<bool> checked;
            checked.resize(tasks_amount,0);
            for(int i = 0;i<tasks_amount;i++){
                reallocateFastest(HARD_TIME,checked);
            }
            
            break;
        }

        case 11:{      
            int HARD_TIME = 10000; 
            for (int task_id = 0; task_id < tasks_amount; ++task_id) { //Lowest TIME
                createInstance(task_id);
                estimatedCost += times.getCost(task_id, getInstance(task_id)->getHardwarePtr());
            }


            std::vector<bool> checked;
            checked.resize(tasks_amount,0);
            for(int i = 0;i<tasks_amount;i++){
                reallocateFastest(HARD_TIME,checked);
            }

            break;
        }

        case 61:{        
            int LOOP_COUNTER;
            int HARD_TIME = 1000;
            int currTask = 0;
            int min_time;
            std::vector<int> bfs_tasks = TaskGraph.BFS();
            constructByWeight(bfs_tasks);
            //reallocateFastest();
            break;
        }

        case 70:{        
            int LOOP_COUNTER;
            int HARD_TIME = 1000;
            int currTask = 0;
            int min_time;
            std::vector<int> bfs_tasks = TaskGraph.BFS();
            for(int currTask : bfs_tasks){
                Hardware* hw = getLowestTimeHardware(currTask,0);
                for(Instance* inst : Instances){
                    if(inst->getHardwarePtr()==hw && getInstanceEndingTime(inst) <= getStartingTime(currTask)){
                        addTaskToInstance(currTask,inst);
                        allocated_tasks[currTask] = 1;
                        break;
                    }
                }
                createInstance(currTask);
            }
            for(int currTask : bfs_tasks){
                getCurrWeight(currTask,1,HARD_TIME);
            }
            //reallocateFastest(HARD_TIME);
            break;
        }

        case 1:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) { //Lowest TIME
                createInstance(task_id);
                estimatedCost += times.getCost(task_id, getInstance(task_id)->getHardwarePtr());
            }
            break;
        }
        case 2:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) { //Lowest COST
                Hardware* hw = getLowestTimeHardware(task_id,1);
                createInstance(task_id,hw);
                estimatedCost += times.getCost(task_id, hw);
            }
            break;
        }
        case 3:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) { //LOWEST TIME z ponownym wykorzystaniem instancji
                int min_time = 2000000;
                Hardware* hw = getLowestTimeHardware(task_id,0);
                bool foundInstance = false;
                for (Instance* inst : Instances) {
                    if (inst->getHardwarePtr() == hw) {    //Jeśli najszybszy HW dla task == dostępny już w instancji to go użyj
                        addTaskToInstance(task_id,inst);
                        foundInstance = true;
                        break;
                    }
                }
                if (!foundInstance) {
                    createInstance(task_id,hw);
                }
                estimatedCost += times.getCost(task_id, hw);
            }
            break;
        }
        case 4:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) { //LOWEST COST z ponownym wykorzystaniem instancji
                int min_time = 2000000;
                Hardware* hw = getLowestTimeHardware(task_id,1);

                bool foundInstance = false;
                for (Instance* inst : Instances) {
                    if (inst->getHardwarePtr() == hw) {
                        addTaskToInstance(task_id,inst);
                        foundInstance = true;
                        break;
                    }
                }
                if (!foundInstance) {
                    createInstance(task_id,hw);
                }
                estimatedCost += times.getCost(task_id, hw);
            }
            break;
        }
        case 40:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) { //LOWEST NORMAIZED z ponownym wykorzystaniem instancji
                int min_time = 2000000;
                Hardware* hw = getLowestTimeHardware(task_id,2);

                bool foundInstance = false;
                for (Instance* inst : Instances) {
                    if (inst->getHardwarePtr() == hw) {
                        addTaskToInstance(task_id,inst);
                        foundInstance = true;
                        break;
                    }
                }
                if (!foundInstance) {
                    createInstance(task_id,hw);
                }
                estimatedCost += times.getCost(task_id, hw);
            }
            break;
        }
        case 41:{
            int LOOP_COUNTER;
            int HARD_TIME = 1000;
            int currTask = 0;
            int min_time;
            std::vector<int> bfs_tasks = TaskGraph.BFS();
            for(int currTask : bfs_tasks){
                Hardware* hw = getLowestTimeHardware(currTask,2);
                for(Instance* inst : Instances){
                    if(inst->getHardwarePtr()==hw && getInstanceEndingTime(inst) <= getStartingTime(currTask)){
                        addTaskToInstance(currTask,inst);
                        allocated_tasks[currTask] = 1;
                        break;
                    }
                }
                if(allocated_tasks[currTask]==0){
                    createInstance(currTask,getLowestTimeHardware(currTask,2));
                    allocated_tasks[currTask] =1;
                }
            }
            times.normalize();

            break;
        }
        case 50:{                //Przydzielanie po koleji, po sąsiadach 0
            int currTask = 0;  
            int min_time = 2000000;
            Hardware* lowestTimeHW = getLowestTimeHardware(0,0);
            createInstance(0,lowestTimeHW);
            allocated_tasks[0] = 1;
            std::vector<int> currSet = TaskGraph.getOutNeighbourIndices(currTask);
            while(currSet.size() != 0){
                currSet = TaskGraph.getOutNeighbourIndices(currTask); 
                min_time = 0;
                for(int t : currSet){
                    if(times.getTime(t,lowestTimeHW)>min_time){ //Najdluzsze zadanie z sasiadów currTask przydzielam do sprzętu wybranego dla CurrTask 
                        min_time = times.getTime(t,lowestTimeHW);
                        currTask = t;
                    }
                }
                if(allocated_tasks[currTask] == 0){ //
                    addTaskToInstance(currTask,Instances[0]);
                    allocated_tasks[currTask] = 1;
                }

                for(int t : currSet){
                    if(allocated_tasks[t] == 0){
                        createInstance(t,getLowestTimeHardware(t,0)); //Dla reszty tworze nowe instancje
                        allocated_tasks[t] = 1;
                    }
                }
            }
            for(int i=0;i<tasks_amount;i++){ //Dla nieprzydzielonych zadań tworze nowe instancje
                if(allocated_tasks[i]==0){
                    createInstance(i,getLowestTimeHardware(i,0));
                    allocated_tasks[i] = 1;
                    //std::cout << "DODAJE " << i << " zad\n";
                }
            }
            int minStartTime = INF;
            int newInstTask = 0;
            bool foundOtherInst = false;
            for(int i=0;i<tasks_amount;i++){
                for(Instance* ins : Instances){ //Jeśli równolegle dostępna jest wolna instancja to przenosze zadanie do niej
                    if (getStartingTime(i) < getInstanceEndingTime(ins)) {
                        removeTaskFromInstance(i);
                        addTaskToInstance(i, ins);
                    }
                }
            }

            break;
        }

        case 5:{
            createInstance(0);
            std::vector<int> allocatedTasks;
            allocatedTasks.resize(tasks_amount,0);
            allocatedTasks[0] = 1;
            for(int i : TaskGraph.BFS()){
                for(Instance* ins : Instances){
                    if(allocatedTasks[i] == 1){
                        //std::cout << "Zadanie " << i << "juz zaalokowane\n";
                        break;
                    }
                    //std::cout << "instancja konczy o " << getInstanceEndingTime(ins) << "a chce zaczac o " <<getStartingTime(i) << "wiec\n";
                    if(getInstanceEndingTime(ins)<=getStartingTime(i) && ins->getHardwarePtr() == getLowestTimeHardware(i,0)){
                        //std::cout << "DOdaje!!!!!!\n";
                        allocatedTasks[i] = 1;
                        addTaskToInstance(i,ins);
                    }
                }
                if(allocatedTasks[i] == 0){
                    createInstance(i);
                    //std::cout << "TWORZE MNOWE!!!!!!\n";
                    allocatedTasks[i] = 1;
                }
            }
            break;
        }

        case 6:{ //GREEDY
            Hardware* FirstLowestTimeHW = getLowestTimeHardware(0, 0);
            std::set<int> allocatedTasks;
            allocatedTasks.clear();
            allocatedTasks.insert(0);
            int currTask = 0;
            Hardware* minHw;
            std::stack<int> toCheck;
            toCheck.push(0);
            int max_task_id = 0;
            int max_time;
            createInstance(currTask, FirstLowestTimeHW);
            while (!toCheck.empty()) {
            max_time = 0;
            for (int i : TaskGraph.getNeighbourIndices(currTask)) {
                toCheck.push(i);
                int curr_time = times.getTime(i,getLowestTimeHardware(i, 0));
                if (curr_time > max_time) {
                    max_time = curr_time;
                    max_task_id = i;
                }
            }
            if(allocatedTasks.find(max_task_id) == allocatedTasks.end()){
                int lowestTimeHW = times.getTime(max_task_id,getLowestTimeHardware(max_task_id, 0));
                for (Instance* ins : Instances) {
                    if (getInstanceEndingTime(ins) < getStartingTime(max_task_id)) {
                        addTaskToInstance(max_task_id, ins);
                        allocatedTasks.insert(max_task_id);
                        break;
                    }
                }
                if (allocatedTasks.find(max_task_id) == allocatedTasks.end()) {
                    createInstance(max_task_id);
                    allocatedTasks.insert(max_task_id);
                }
            }

            for (int i : TaskGraph.getNeighbourIndices(currTask)) {
                if (i != max_task_id && allocatedTasks.find(i) == allocatedTasks.end()) {
                    int lowestTimeHW = times.getTime(i,getLowestTimeHardware(i, 0));
                    for (Instance* ins : Instances) {
                        if (getInstanceEndingTime(ins) < getStartingTime(i)) {
                        addTaskToInstance(i, ins);
                        allocatedTasks.insert(i);
                        break;
                    }
                    }
                    if (allocatedTasks.find(i) == allocatedTasks.end()) {
                        createInstance(i);
                        allocatedTasks.insert(i);
                    }
                }
            }
            currTask = toCheck.top();
            toCheck.pop();
            }
            break;
        }
        case 7:{
            createInstance(0);

            allocated_tasks[0] = 1;
            for(int i : TaskGraph.BFS()){
                for(Instance* ins : Instances){
                    if(allocated_tasks[i] == 1){
                        //std::cout << "Zadanie " << i << "juz zaalokowane\n";
                        break;
                    }
                    //std::cout << "instancja konczy o " << getInstanceEndingTime(ins) << "a chce zaczac o " <<getStartingTime(i) << "wiec\n";
                    if(getInstanceEndingTime(ins)<=getStartingTime(i)){
                        //std::cout << "DOdaje!!!!!!\n";
                        allocated_tasks[i] = 1;
                        addTaskToInstance(i,ins);
                    }
                }
                if(allocated_tasks[i] == 0){
                    createInstance(i);
                    //std::cout << "TWORZE MNOWE!!!!!!\n";
                    allocated_tasks[i] = 1;
                }
            }
            break;
        }
        case 9:{
            createInstance(0);
            for(int t = 0; t<tasks_amount;t++){
                addTaskToInstance(t,getInstance(0));
            }
            break;}
        default:{
            std::cerr << "Nieznana reguła dystrybucji zadań\n";
            break;}
    }
    printUnpredictedTasks();
    printSchedule();
    printInstances();
}
