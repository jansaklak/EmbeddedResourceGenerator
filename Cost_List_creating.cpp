
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



int Cost_List::Load_From_File(const std::string& filename) {
    clear();
    clearNUM();
    tasks_amount = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return -1;
    }
    std::string line;
    int section = -1;
    int task_id,weight,to,tasks;
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
                while(ss >> c >> task_id >> tasks){
                    int value;
                    while (ss >> to >> c >> weight >> c) {
                        if(weight == 0) weight = 1;
                        loaded.addEdge(task_id,to,weight);
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