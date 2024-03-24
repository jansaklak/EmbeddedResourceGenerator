#include "Cost_List.h"
#include "Times.h"
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include "TimeAndCost.h"
#include <mutex>
#include <algorithm>

bool randomBool(int prob) {
    prob++;
    return rand() % prob == 0;
}

int getRand(int MAX){
    if(MAX < 1){
        MAX = 1;
    }
    return rand() % MAX;
}

std::vector<Hardware> Cost_List::getHardwares(){
    return Hardwares;
}

std::vector<COM> Cost_List::getCOMS(){
    return Channels;
}

Graf Cost_List::getGraph(){
    return TaskGraph;
}
Times Cost_List::getTimes(){
    return times;
}

Cost_List::Cost_List(){
    tasks_amount = 0;
    hardware_cores_amount = 0;
    processing_unit_amount = 0;
    channels_amount = 0;
    with_cost = 0;
}

Cost_List::Cost_List(int _tasks,int _hcores,int _punits,int _channels,int _withCost){
    tasks_amount = _tasks;
    hardware_cores_amount = _hcores;
    processing_unit_amount = _punits;
    channels_amount = _channels;
    with_cost = _withCost;
}

void Cost_List::Load_From_File(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Nie udało się otworzyć pliku." << std::endl;
        return;
    }
    std::string line;
    int section = -1;
    int Tnum,weight,to,tasks;
    int HW_cost,HW_type;
    int HW_ID_counter = 0;
    std::vector<std::vector<int>> times_matrix;
    std::vector<std::vector<int>> costs_matrix;
    std::vector<int> row_times;
    std::vector<int> row_costs;
    Graf loaded;
    while (getline(file, line)) {
         if (line.find("@tasks") != std::string::npos) {
            section = 0;
            std::cout << "Sekcja @tasks" << std::endl;
        } else if (line.find("@proc") != std::string::npos) {
            section = 1;
            std::cout << "Sekcja @proc" << std::endl;
        } else if (line.find("@times") != std::string::npos) {
            section = 2;
            std::cout << "Sekcja @times" << std::endl;
        } else if (line.find("@cost") != std::string::npos) {
            section = 3;
            std::cout << "Sekcja @cost" << std::endl;
        } else if (line.find("@comm") != std::string::npos) {
            section = 4;
            std::cout << "Sekcja @comm" << std::endl;
        } else {
            if (section != -1) {
                //std::cout << "Linia w sekcji " << section << ": " << line << std::endl;
            }
            if(section == 0){
                char c;
                std::stringstream ss(line);
                while(ss >> c >> Tnum >> tasks){
                    int value;
                    while (ss >> to >> c >> weight >> c) {
                        std::cout << "dodaje wektor" << Tnum << " " << to << " "  << weight <<std::endl;
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
            if(section ==2 ){
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
                            c.add_Hardware(Hardwares[i]);
                        }
                        i++;
                    }
                    Channels.push_back(c);
                }
                times.setTimesMatrix(times_matrix);
                times.setCostsMatrix(costs_matrix);
                TaskGraph = loaded;
                TaskGraph.PrintToFile(Tnum,"matrix_test.dat");      
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
    for(int i : visited) {
        for (int j = 0; j < tasks_amount; j++) {
            if (randomBool(tasks_amount / 6) && !visited.count(j)) {
                if(with_cost) {
                    G.addEdge(i, j, getRand(SCALE));
                } else {
                    G.addEdge(i, j);
                }
                visited.insert(j);
            }
        }
    }
    for (int i = 0; i < tasks_amount; ++i) {
        if(visited.find(i) == visited.end()){
            int random_number = getRand(tasks_amount);
            while(random_number == i){
                random_number = getRand(tasks_amount);
            }
            if(with_cost){
                G.addEdge(random_number, i, getRand(SCALE));
            } else {
                G.addEdge(random_number, i);
            }
        }
    }
    TaskGraph = G;
    return;
}

void Cost_List::printTasks(std::ostream& out) {
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

void Cost_List::PrintCOMS(std::ostream& out) {
    out << "@comm " << Channels.size() <<"\n";
    for(COM c : Channels){
        c.PrintCOM(Hardwares.size(),out);
    }
}

void Cost_List::PrintProc(std::ostream& out) {
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

void Cost_List::PrintALL(std::string filename,bool toScreen){
    std::ofstream outputFile(filename); 
            if (outputFile.is_open()) {
                std::ofstream outputFileMatrix("matrix.dat"); 
                TaskGraph.printMatrix(TaskGraph.getVerticesSize(),outputFileMatrix);
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

void Cost_List::getRandomProc(){
            for(int i = 0; i<hardware_cores_amount;i++){
                Hardwares.push_back(Hardware(getRand(5)+1,Hardware_Type::HC,Hardwares.size()));
            }
            for(int i = 0; i<processing_unit_amount;i++){
                Hardwares.push_back(Hardware(getRand(5)+1,Hardware_Type::PE,Hardwares.size()));
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
                        c.add_Hardware(h);
                    }
                }

                while(c.getSize()<3){
                        rd = getRand(Hardwares.size());
                        c.add_Hardware(Hardwares[rd]);
                }
                Channels.push_back( c );
            }
            
            for(Hardware h : Hardwares){
                bool All_connected = false;
                for(COM c : Channels){
                    if(c.isConnected(h.getID())){
                        All_connected = true;
                    }
                }
                if(All_connected == false){
                        Channels[0].add_Hardware(h);
                    }
            }

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

void Licznik(bool& stop, int& czas) {
    auto start = std::chrono::steady_clock::now(); // Początkowy punkt czasowy

    while (!stop) {
        auto current = std::chrono::steady_clock::now();
        
        czas = std::chrono::duration_cast<std::chrono::seconds>(current - start).count();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void Cost_List::RunTasks() {
    int TotalCost = 0;
    std::cout << "\nUruchamiam zadania\n";
    progress.resize(TaskGraph.getVerticesSize(), -2); // -2 - cant be done flag
    progress[0] = -1;
    std::vector<std::thread> threads;
    int numThreads = Hardwares.size();

    bool stop = false; 
    int czas = 0;
    std::thread counterThread(Licznik, std::ref(stop), std::ref(czas));

    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(&Cost_List::TaskRunner, this, i));
    }
    for (std::thread& thread : threads) {
        thread.join();
    }
    
    stop = true;
    counterThread.join();

    std::cout << "Czas trwania programu: " << czas << " sekund.\n";
}

void Cost_List::TaskDistribution(int rule) {
    HWtoTasks.resize(Hardwares.size());
    int estimatedCost = 0;
    int estimatedTime = 0;
    
    int tasks_amount = TaskGraph.getVerticesSize();
    if(rule < 3){                                                                            //WG NAJMNIEJSZEGO POJEDYNCZEGO
        for (int task_id = 0; task_id < tasks_amount; ++task_id) {
            int min_time = 2000000;
            int min_hw_id = 0;
            for (Hardware hw : Hardwares) {
                int time;
                if(rule == 0){                                                               //CZASU
                    time = times.getTime(task_id,hw.getID());
                }
                else{                                                                           //KOSZTU
                    time = times.getCost(task_id,hw.getID());
                }

                if (time < min_time) {
                    min_time = time;
                    min_hw_id = hw.getID();
                }
            }
            HWtoTasks[min_hw_id].insert(task_id);
            estimatedCost += times.getCost(task_id,min_hw_id);
        }
    }
    else if(rule > 2){                                                                 // Równo na każdy HW
        int currHw= 0;
        for (int task_id = 0; task_id < tasks_amount; ++task_id) {
            HWtoTasks[currHw].insert(task_id);
            estimatedCost += times.getCost(task_id,currHw);
            currHw++;
            if(currHw == Hardwares.size()){
                currHw = 0;
            }
        }
    }
    int j = 0;
    std::cout << "Podział zadań: \n";
    std::cout << "Spodziewany calkowity koszt: " << estimatedCost <<std::endl;
    int SumTime = 0;
    int SumCost = 0;
    for(std::set<int> hw : HWtoTasks){
        SumTime = 0;
        SumCost = 0;
        std::cout << "HW" << j << ": " << hw.size();
        for(int k : hw){
            SumTime += times.getTime(k,j);
            SumCost += times.getCost(k,j);
        }
        std::cout << "Spodziewany czas pracy: " << SumTime << " koszt: " << SumCost <<std::endl;
        j++;
    }
    

}

void Cost_List::TaskRunner(int HWID) {
    int taskCounter = 0;
    while (true) {
        int alltasks = HWtoTasks[HWID].size();
        for (int t : HWtoTasks[HWID]) {
            if (progress[t] == -1) {
                int time = times.getTime(t, HWID);
                {
                    std::cout << "T" << t << " on HW" << HWID << std::endl;
                    progress[t] = 0;
                    for (int i = 1; i < time+1; ++i) {
                        progress[t] = (((i + 1) * 100) / time);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                    ++taskCounter;
                    std::cout << "\tT" << t << "done\n";
                    for (int i : TaskGraph.getOutNeighbourIndices(t)) {
                        if(progress[i]==-2) progress[i] = -1;//READY TO RUN
                    }
                }
            }
        }
        if (taskCounter == alltasks) {
            std::cout << "\t\tHW" << HWID << " ended\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return;
}



