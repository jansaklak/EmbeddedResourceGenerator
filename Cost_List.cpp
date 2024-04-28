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


void Licznik(bool& stop, int& czas) {
    auto start = std::chrono::steady_clock::now();
    while (!stop) {
        auto current = std::chrono::steady_clock::now();
        czas = std::chrono::duration_cast<std::chrono::milliseconds>(current - start).count();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

bool randomBool(int prob) {
    prob++;
    return rand() % prob == 0;
}

int getRand(int MAX){
    if(MAX < 1) MAX = 1;
    return rand() % MAX;
}

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
}

int Cost_List::Load_From_File(const std::string& filename) {
    clear();
    tasks_amount = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return -1;
    }
    std::string line;
    int section = -1;
    int Tnum,weight,to,tasks;
    int HW_cost,HW_type;
    std::vector<std::vector<int>> times_matrix;
    std::vector<std::vector<int>> costs_matrix;
    std::vector<int> row_times;
    std::vector<int> row_costs;
    Graf loaded;
    int HW_ID_counter = 0;
    while (getline(file, line)) {
         if (line.find("@tasks") != std::string::npos) {
            section = 0;
            //std::cout << "Sekcja @tasks" << std::endl;
        } else if (line.find("@proc") != std::string::npos) {
            section = 1;
            //std::cout << "Sekcja @proc" << std::endl;
        } else if (line.find("@times") != std::string::npos) {
            section = 2;
            //std::cout << "Sekcja @times" << std::endl;
        } else if (line.find("@cost") != std::string::npos) {
            section = 3;
            //std::cout << "Sekcja @cost" << std::endl;
        } else if (line.find("@comm") != std::string::npos) {
            section = 4;
            //std::cout << "Sekcja @comm" << std::endl;
        } else {
            if (section != -1) {
            }
            if(section == 0){
                char c;
                std::stringstream ss(line);
                while(ss >> c >> Tnum >> tasks){
                    int value;
                    while (ss >> to >> c >> weight >> c) {
                        if(weight == 0) weight = 1;
                        loaded.addEdge(Tnum,to,weight);
                        tasks_amount++;
                    }
                }
            }
            if(section == 1){
                char c;
                std::stringstream ss(line);
                while(ss >> HW_cost >> to >> HW_type){
                    Hardware hw = Hardware(HW_type,HW_cost,HW_ID_counter);
                    Hardwares.push_back(hw);
                    HW_ID_counter++;
                }
            }
            if(section == 2){
                std::istringstream iss(line);
                int value;
                while (iss >> value) {
                    row_times.push_back(value);
                }
                times_matrix.push_back(row_times);
                row_times.clear();
            }
            if(section ==3 ){
                std::istringstream iss(line);
                int value;
                while (iss >> value) {
                    row_costs.push_back(value);
                }
                costs_matrix.push_back(row_costs);
                row_costs.clear();
            }
            if(section == 4){
                char c;
                int chanNum;
                int ComCost;
                int BandWidth;
                int connection;
                int i =0;
                std::stringstream ss(line);
                while(ss >> c >> c >> c >> c >> chanNum >> ComCost >> BandWidth){
                    COM c = COM(ComCost,BandWidth,chanNum);
                    while(ss >> connection){
                        if(connection == 1){
                            c.add_Hardware(&Hardwares[i]);
                        }
                        i++;
                    }
                    Channels.push_back(c);
                }
                times.setTimesMatrix(times_matrix);
                times.setCostsMatrix(costs_matrix);
                TaskGraph = loaded;
                // std::ofstream matrix_file("matrix.dat");
                // if (!matrix_file.is_open()) {
                //     return -1;
                // }
                // TaskGraph.printMatrix(matrix_file);
            }
        }
    }
    tasks_amount--;
    file.close();
    return 1;
}

void Cost_List::createRandomTasksGraph() {
    if (tasks_amount <= 1) {
        std::cerr << "Błędna liczba zadań";
        return;
    }
    //std::cout << "TWORZĘ WYKRES DO " << tasks_amount << std::endl;
    std::set<int> visited;
    Graf G;
    if (with_cost) {
        G.addEdge(0, 1, getRand(SCALE));
    } else {
        G.addEdge(0, 1);
    }
    visited.insert(0);
    for (int i : visited) {
        for (int j = 0; j < tasks_amount; j++) {
            if (randomBool(tasks_amount / 6) && !visited.count(j)) {
                if (with_cost) {
                    G.addEdge(i, j, getRand(SCALE));
                } else {
                    G.addEdge(i, j);
                }
                visited.insert(j);
            }
        }
    }

    for (int i = 0; i < tasks_amount; ++i) {
        if (visited.find(i) == visited.end()) {
            //std::cout << "NIE DODANO " << i << std::endl;
            int idx = rand() % visited.size();
            auto it = visited.begin();
            for (int k = 0; k < idx; k++) {
                it++;
            }
            int random_visited_task = *it;
            //std::cout << "RANDOM VISITED = " << random_visited_task << "\n";

            if (!G.checkEdge(random_visited_task, i)) {
                if (with_cost) {
                    G.addEdge(random_visited_task, i, getRand(SCALE));
                } else {
                    G.addEdge(random_visited_task, i);
                }
                visited.insert(i);
            }
        }
    }
    //std::cout << "tutut***" << std::endl;
    visited.clear();
    TaskGraph = G;
    //std::cout << "SKOŃCZYŁEM" << std::endl;
    return;
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
        c.printCOM(Hardwares.size(),out);
    }
}

void Cost_List::printProc(std::ostream& out) const{
    out << "@proc " << Hardwares.size() << std::endl;
    for (Hardware h : Hardwares) {
        h.printHW(out);
         out << std::endl;
    }
}

int Cost_List::getCriticalTime(){
    int maxTime = 0;
    for(Instance* i : Instances){
        if(getInstanceEndingTime(i)>maxTime) maxTime = getInstanceEndingTime(i);
    }
    return maxTime;
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

void Cost_List::printInstances(){
    std::sort(Instances.begin(), Instances.end());
    std::cout << "Stworzono " << Instances.size() << " komponentów\n";
    int task_id;
    int task_time;
    int totalCost = 0;
    printSchedule();
    
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
        totalCost += expCost;
        std::cout << *inst << " Zadan: " << inst->getTaskSet().size() << " Spodziewany czas: " << expTime << " Bezczynnosci: " << getIdleTime(inst,criticTime) << " koszt: " << expCost  << " w tym początkowy: " << inst->getHardwarePtr()->getCost() << "\n";
    }
    std::cout << "Czas scieżki krytycznej: " << criticTime;
    std::cout << "\tKoszt całkowity: " << totalCost << std::endl;

}

void Cost_List::randALL(){
    times = Times(tasks_amount);
    createRandomTasksGraph();
    getRandomProc();
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

int Cost_List::getRandomProc() {
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
    for(int i = 0; i<channels_amount;i++){
        int bd = ((getRand(9)+1)*10);
        int con_cost = (getRand(9)+1)*5;
        int rd;
        COM c = COM(bd,con_cost,Channels.size());
        for(Hardware h : Hardwares){
            if(randomBool(i+1)){
                c.add_Hardware(&h);
            }
        }
        while(c.getSize()<3){
                rd = getRand(Hardwares.size());
                c.add_Hardware(&Hardwares[rd]);
        }
        Channels.push_back( c );
    }
    for(Hardware h : Hardwares){
        bool All_connected = false;
        for(COM c : Channels){
            if(c.isConnected(&h)){
                All_connected = true;
            }
        }
        if(All_connected == false){
                Channels[0].add_Hardware(&h);
            }
        //std::cout << " ROBIE";
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

int Cost_List::createInstance(int task_ID, const Hardware* h) {
    if(allocated_tasks[task_ID]==1){
        return 0;
    }
    Instance* newInst = new Instance(HWInstancesCount[h->getID()], h);
    HWInstancesCount[h->getID()]++;
    newInst->addTask(task_ID);
    Instances.push_back(newInst);
    taskInstanceMap[task_ID] = newInst;
    return 1;
}

int Cost_List::createInstance(int task_ID) {
    if (allocated_tasks[task_ID] == 1) {
                return 0;
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
    taskInstanceMap[task_ID] = inst;
    allocated_tasks[task_ID] = 1;

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
    Instance* oldInst = taskInstanceMap[task_ID];
    oldInst->removeTask(task_ID);
    if(oldInst->getTaskSet().empty()){
        Instances.erase(std::remove(Instances.begin(), Instances.end(), oldInst), Instances.end());
    } 
    oldInst=nullptr;
    return;
}

void Cost_List::TaskRunner(Instance i) {
    int taskCounter = 0;
    const Hardware* running_hw = i.getHardwarePtr();
    while (true) {
        int alltasks = i.getTaskSet().size();
        for (int t : i.getTaskSet()) {
            if (progress[t] == -1) {
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
                    for (int i : TaskGraph.getOutNeighbourIndices(t)) {
                        if(progress[i]==-2) progress[i] = -1;
                    }
                }
            }
        }
        if (taskCounter == alltasks) {
            std::cout << "\t\t\t" << i << " ended\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return;
}

void Cost_List::printSchedule() {
    int instance_ending_time;
    for(Instance* i : Instances) {
        instance_ending_time = 0;
        int alltasks = i->getTaskSet().size();
        for (int t : i->getTaskSet()) {
                int time = times.getTime(t, i->getHardwarePtr());
                if(getInstanceStartingTime(i)>=getStartingTime(t)+time){
                    std::pair<int,int> timeRunning = {getStartingTime(t),getStartingTime(t)+time};
                    task_schedule.insert({t,timeRunning});
                }
                else if(instance_ending_time<=getStartingTime(t)){
                    //std::cout << "dla " << t << "ZAMIENIAN " << instance_ending_time << " NA" << getStartingTime(t) << "\n";
                    instance_ending_time = getStartingTime(t);
                    std::pair<int,int> timeRunning = {getStartingTime(t),getStartingTime(t)+time};
                    task_schedule.insert({t,timeRunning});
                    instance_ending_time = getStartingTime(t);
                }
                // if()
                // std::pair<int,int> timeRunning = {instance_ending_time,instance_ending_time+time};
                // task_schedule.insert({t,timeRunning});
                // //std::cout << "od" <<timeRunning.first << "do" << timeRunning.second <<"\n";

                instance_ending_time += time;
                //std::cout << "instancja konczy o " << instance_ending_time << "\n";
        }
    }

    // std::cout << "Porządek topologiczny: * - stan końcowy, () - czas na najszybszym HW\n";
    // for(int i : getLongestPath(0)){
    //     std::cout << i <<"(" << times.getTime(i,getLowestTimeHardware(i,0)) <<")";
    //     if(TaskGraph.getOutNeighbourIndices(i).size()==0){
    //         std::cout <<"*";
    //     }
    //     std::cout <<"-";
    // }
    std::cout << "\n";
    return;
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

Instance* Cost_List::getInstance(int task_id){
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
            }
        }
        if (skipPath) {
            continue; // Przechodzimy do następnej ścieżki, pomijając aktualną
        }
        if (pathTime < lowestTime) {
            if(task_id == 45){
                std::cout << "Mozliwy czas " << pathTime;
            }
            lowestTime = pathTime;
            bestPath = path;
        }
    }
    return lowestTime;
}


int Cost_List::getEndingTime(int task_id){
    int runningTime = times.getTime(task_id,getInstance(task_id)->getHardwarePtr());
    return getStartingTime(task_id) + runningTime;
}

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