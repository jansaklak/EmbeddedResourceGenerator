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
#define INF 2147483647

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
}

Cost_List::Cost_List(int _tasks, int _hcores, int _punits, int _channels, int _withCost)
    : tasks_amount(_tasks), hardware_cores_amount(_hcores),
    processing_unit_amount(_punits), channels_amount(_channels),
    with_cost(_withCost) {
    Hardwares.clear();
    Channels.clear();
    times = Times();
}

void Cost_List::Load_From_File(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku." << std::endl;
        return;
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
                std::ofstream matrix_file("matrix.dat");
                if (!matrix_file.is_open()) {
                    std::cout << "Nie udało sis otworzyć pliku." << std::endl;
                    return;
                }
                TaskGraph.printMatrix(matrix_file);
            }
        }
    }
    file.close();
}

void Cost_List::CreateRandomTasksGraph() {
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
            int random_visited_task = *visited.begin();
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
        TaskGraph = G;
    std::ofstream matrix_file("matrix.dat");
        if (!matrix_file.is_open()) {
            std::cout << "Nie udało sis otworzyć pliku." << std::endl;
            return;
        }
    TaskGraph.printMatrix(matrix_file);
    return;
}

void Cost_List::printTasks(std::ostream& out) const{
    out << "@tasks " << TaskGraph.getVerticesSize() << std::endl;
    std::vector<int> outIdx;
    int size = TaskGraph.getVerticesSize();
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

void Cost_List::PrintCOMS(std::ostream& out) const{
    out << "@comm " << Channels.size() <<"\n";
    for(COM c : Channels){
        c.PrintCOM(Hardwares.size(),out);
    }
}

void Cost_List::PrintProc(std::ostream& out) const{
    out << "@proc " << Hardwares.size() << std::endl;
    for (Hardware h : Hardwares) {
        h.PrintHW(out);
         out << std::endl;
    }
}

void Cost_List::RandALL(){
    CreateRandomTasksGraph();
    getRandomProc();
    times = Times(TaskGraph.getVerticesSize(),Hardwares);
    times.SetRandomTimesAndCosts();
    ConnectRandomCH();
}

void Cost_List::PrintALL(std::string filename,bool toScreen) const{
    std::ofstream outputFile(filename, std::ofstream::trunc); 
    if (outputFile.is_open()) {
        std::ofstream outputFileMatrix("matrix.dat", std::ofstream::trunc); 
        TaskGraph.printMatrix(outputFileMatrix);
    //@tasks
        if(toScreen) printTasks();
        printTasks(outputFile);
    //@proc
        if(toScreen) PrintProc();
        PrintProc(outputFile);
    //@times @cost
        if(toScreen) times.show();
        times.show(outputFile);
    //@coms
        PrintCOMS(outputFile);
        if(toScreen) PrintCOMS();
        outputFile.close();
        std::cout << "Zapisano liste do pliku " << filename << std::endl;
    } 
    else {
        std::cerr << "Nie można otworzyć pliku do zapisu." << std::endl;
        return;
    }
}

void Cost_List::getRandomProc() {
    for(int i = 0; i<hardware_cores_amount;i++){
        Hardwares.push_back(Hardware(getRand(5)+1,Hardware_Type::HC,Hardwares.size()));
    }
    int HC_size = Hardwares.size();
    for(int i = 0; i<processing_unit_amount;i++){
        Hardwares.push_back(Hardware(getRand(5)+1,Hardware_Type::PE,Hardwares.size()-HC_size));
    }
    return;
}

void Cost_List::ConnectRandomCH(){
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
            }
}

void Licznik(bool& stop, int& czas) {
    auto start = std::chrono::steady_clock::now();
    while (!stop) {
        auto current = std::chrono::steady_clock::now();
        czas = std::chrono::duration_cast<std::chrono::milliseconds>(current - start).count();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

void Cost_List::RunTasks() {
    int TotalCost = 0;
    std::cout << "\nUruchamiam zadania:\n";
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
    std::cout << "\n\nCzas trwania programu: " << czas << " milisekund.\n\n";
}


Hardware* Cost_List::getLowestTimeHardware(int task_id, int time_cost) const{
    Hardware* outHW = nullptr;
    int min_time = INF;
    for (const Hardware& hw : Hardwares) {
        int time;
        if(time_cost == 0){
            time = times.getTime(task_id, &hw);
        }
        else{
            time = times.getCost(task_id, &hw);
        }
        if (time < min_time) {
            min_time = time;
            outHW = &const_cast<Hardware&>(hw);
        }
    }
    return outHW;
}

void Cost_List::createInstance(int task_ID, const Hardware* h) {
    Instance* newInst = new Instance(HWInstancesCount[h->getID()], h);
    HWInstancesCount[h->getID()]++;
    newInst->addTask(task_ID);
    Instances.push_back(newInst);
    taskInstanceMap[task_ID] = newInst;
}

void Cost_List::createInstance(int task_ID) {
    Hardware* h = getLowestTimeHardware(task_ID,0);
    Instance* newInst = new Instance(HWInstancesCount[h->getID()], h);
    HWInstancesCount[h->getID()]++;
    newInst->addTask(task_ID);
    Instances.push_back(newInst);
    taskInstanceMap[task_ID] = newInst;
}

void Cost_List::addTaskToInstance(int task_ID, Instance* inst) {
    inst->addTask(task_ID);
    taskInstanceMap[task_ID] = inst;
}

void Cost_List::removeTaskFromInstance(int task_ID){
    Instance* oldInst = taskInstanceMap[task_ID];
    oldInst->removeTask(task_ID);
    if(oldInst->getTaskSet().empty()) Instances.erase(std::remove(Instances.begin(), Instances.end(), oldInst), Instances.end());
    taskInstanceMap[task_ID] = nullptr;
}

void Cost_List::TaskDistribution(int rule) {
    int estimatedCost = 0;
    int estimatedTime = 0;
    TotalCost = 0;
    int tasks_amount = TaskGraph.getVerticesSize();

    // std::cout << "Porządek topologiczny: * - stan końcowy, () - czas na najszybszym HW\n";
    // for(int i : getLongestPath(0)){
    //     std::cout << i <<"(" << times.getTime(i,getLowestTimeHardware(i,0)) <<")";
    //     if(TaskGraph.getOutNeighbourIndices(i).size()==0){
    //         std::cout <<"*";
    //     }
    //     std::cout <<"-";
    // }
    // std::cout << "\n";

    switch (rule) {
        case 0:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) {
                Hardware* hw = getLowestTimeHardware(task_id,0);
                createInstance(task_id,hw);
                estimatedCost += times.getCost(task_id, hw);
            }
            break;
        }
        case 1:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) {
                Hardware* hw = getLowestTimeHardware(task_id,1);
                createInstance(task_id,hw);
                estimatedCost += times.getCost(task_id, hw);
            }

            break;
        }
        case 2:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) {
                int min_time = 2000000;
                Hardware* hw = getLowestTimeHardware(task_id,0);

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
        case 3:{
            for (int task_id = 0; task_id < tasks_amount; ++task_id) {
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
        case 4:{
            bool allocated_tasks[tasks_amount] = {false};
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
                    if(times.getTime(t,lowestTimeHW)>min_time){
                        min_time = times.getTime(t,lowestTimeHW);
                        currTask = t;
                    }
                }
                if(allocated_tasks[currTask] == 0){
                    addTaskToInstance(currTask,Instances[0]);
                    allocated_tasks[currTask] = 1;
                }
                
                for(int t : currSet){
                    if(allocated_tasks[t] == 0){
                        createInstance(t,getLowestTimeHardware(t,0));
                        allocated_tasks[t] = 1;
                    }
                }
                
            }
            for(int i=0;i<tasks_amount;i++){
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
                for(Instance* ins : Instances){
                    if (getStartingTime(i) < getInstanceEndingTime(ins)) {
                        removeTaskFromInstance(i);
                        addTaskToInstance(i, ins);
                    }
                }
                }
            
            break;
        }
        case 5:{ //GREEDY
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
                        break; // Task assigned, no need to continue searching for instances
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
        }
        case 6:{
            createInstance(0);
            int allocatedTasks[tasks_amount] = {0};
            allocatedTasks[0] = 1;
            for(int i : TaskGraph.BFS()){
                for(Instance* ins : Instances){
                    if(allocatedTasks[i] == 1){
                        //std::cout << "Zadanie " << i << "juz zaalokowane\n";
                        break;
                    }
                    //std::cout << "instancja konczy o " << getInstanceEndingTime(ins) << "a chce zaczac o " <<getStartingTime(i) << "wiec\n";
                    if(getInstanceEndingTime(ins)<=getStartingTime(i)){
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
        default:{
            std::cerr << "Nieznana reguła dystrybucji zadań\n";
            break;}
    }
    std::sort(Instances.begin(), Instances.end());
    std::cout << "Stworzono " << Instances.size() << " komponentów\n";
    for (const Instance* inst : Instances) {
        int expTime = 0;
        int expCost = 0;
        for (int taskID : inst->getTaskSet()) {
            expTime += times.getTime(taskID, inst->getHardwarePtr());
            expCost += times.getCost(taskID, inst->getHardwarePtr());
        }
        expCost += inst->getHardwarePtr()->getCost();
        std::cout << *inst << " Zadan: " << inst->getTaskSet().size() << " Spodziewany czas: " << expTime << " koszt: " << expCost  << " w tym początkowy: " << inst->getHardwarePtr()->getCost() << "\n";
    }
    for(auto it = taskInstanceMap.begin(); it != taskInstanceMap.end(); ++it) {
        int task_id = it->first;
        const Instance& inst = *(it->second);
        std::cout << "T" << task_id << " on " << inst << " Running from: " << getStartingTime(task_id) << " to :" << getEndingTime(task_id) << "\n";
    }
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
                    std::cout << ""<< t << " on " << i << std::endl;
                    progress[t] = 0;
                    for (int i = 1; i < time+1; ++i) {
                        progress[t] = (((i + 1) * 100) / time);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

int Cost_List::getInstanceEndingTime(const Instance* inst){
    int endingTime= 0;
    for(int i : inst->getTaskSet()){
        //std::cout << i << "_";
        if(getEndingTime(i)>endingTime) endingTime = getEndingTime(i);
    }
    return endingTime;
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
    //std::cout << " DLA T: " << task_id << "__";
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
