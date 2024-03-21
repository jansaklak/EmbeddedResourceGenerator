#include "Cost_List.h"
#include "Times.h"

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

Cost_List::Cost_List(){
    tasks_amount = 0;
    hc_amount = 0;
    pu_amount = 0;
    ch_amount = 0;
    with_cost = 0;
    toScreen = 0;
}

Cost_List::Cost_List(int tasks,int hcores,int pus,int channels,int withcost,int toscreen){
    tasks_amount = tasks;
    hc_amount = hcores;
    pu_amount = pus;
    ch_amount = channels;
    with_cost = withcost;
    toScreen = toscreen;

}

void Cost_List::Load_From_File(const std::string& filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Nie udało się otworzyć pliku." << std::endl;
        return;
    }

    std::string line;
    int section = -1; // Domyślna wartość dla sekcji, jeśli nie zostanie znaleziona żadna sekcja
    int Tnum,taskts,weight,to,tasks;
    int hw_cost,hw_type;
    int count_hw_id = 0;
    std::vector<std::vector<int>> times;
    std::vector<std::vector<int>> costs;
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
        } else if (line.find("@costs") != std::string::npos) {
            section = 3;
            std::cout << "Sekcja @costs" << std::endl;
        } else if (line.find("@comm") != std::string::npos) {
            section = 4;
            std::cout << "Sekcja @comm" << std::endl;
        } else {
            if (section != -1) {
                std::cout << "Linia w sekcji " << section << ": " << line << std::endl;
            }
            if(section == 0){
                char c;
                std::stringstream ss(line);
                while(ss >> c >> Tnum >> tasks){
                    int value;
                    loaded.createVertices(Tnum + 1);
                    while (ss >> to >> c >> weight >> c) {
                        loaded.addEdge(Tnum,to,weight);
                    }
                }
            }
            if(section == 1){
                
                char c;
                std::stringstream ss(line);
                
                while(ss >> hw_cost >> to >> hw_type){
                    HW_vec.push_back(Hardware(hw_type,hw_cost,count_hw_id));
                    count_hw_id++;
                }
            }
            if(section ==2 ){
                std::istringstream iss(line);
                int value;
                while (iss >> value) {
                    row_times.push_back(value);
                }
                times.push_back(row_times);
                row_times.clear();
            }
            if(section ==3 ){
                std::istringstream iss(line);
                int value;
                while (iss >> value) {
                    row_costs.push_back(value);
                }
                costs.push_back(row_costs);
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
                            c.add_Hardware(HW_vec[i]);
                        }
                        i++;
                    }
                    ch_vec.push_back(c);
                }

                
                t.setTimesMatrix(times);
                t.setCostsMatrix(costs);
                TaskGraph = loaded;
                TaskGraph.PrintToFile(Tnum,"matrix_test.dat");
                
            }
            }
        }
    
    file.close(); // Zamknięcie pliku
}

void Cost_List::CreateRandomTasksGraph() {
    std::set<int> visited;
    Graf G;
    G.createVertices(tasks_amount);
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
    out << "@tasks " << TaskGraph.getVerticlesSize() << std::endl;
    std::vector<int> outIdx;
    int size = TaskGraph.getVerticlesSize();
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
    out << "@comm " << ch_vec.size() <<"\n";
    for(COM c : ch_vec){
        c.PrintCOM(HW_vec.size(),out);
    }
}

void Cost_List::PrintProc(std::ostream& out) {
    out << "@proc " << HW_vec.size() << std::endl;
    for (Hardware h : HW_vec) {
        h.PrintHW(out);
         out << std::endl;
    }
}

void Cost_List::RandALL(){
    CreateRandomTasksGraph();
    getRandomProc();
    t = Times(TaskGraph.getVerticlesSize(),HW_vec);
    t.SetRandomTimesAndCosts();
    ConnectRandomCH();
}

void Cost_List::PrintALL(std::string filename){
    std::ofstream outputFile(filename); 
            if (outputFile.is_open()) {
                std::ofstream outputFileMatrix("matrix.dat"); 
                TaskGraph.printMatrix(tasks_amount,outputFileMatrix);
                //@tasks
                if(toScreen) printTasks();
                printTasks(outputFile);
                //@proc
                if(toScreen) PrintProc();
                PrintProc(outputFile);
                //@times @cost
                
                if(toScreen) t.show();
                t.show(outputFile);
                //@coms
                PrintCOMS(outputFile);
                if(toScreen) PrintCOMS();
                outputFile.close();
            } 
            else {
                std::cerr << "Nie można otworzyć pliku do zapisu." << std::endl;
                return;
            }
}

void Cost_List::getRandomProc(){
            for(int i = 0; i<hc_amount;i++){
                HW_vec.push_back(Hardware(getRand(5)+1,Hardware_Type::HC,HW_vec.size()));
            }
            for(int i = 0; i<pu_amount;i++){
                HW_vec.push_back(Hardware(getRand(5)+1,Hardware_Type::PE,HW_vec.size()));
            }
            return;
        }

void Cost_List::ConnectRandomCH(){
           
            for(int i = 0; i<ch_amount;i++){
                int bd = ((getRand(9)+1)*10);
                int con_cost = (getRand(9)+1)*5;
                int rd;
                COM c = COM(bd,con_cost,ch_vec.size());
                for(Hardware h : HW_vec){
                    if(randomBool(i+1)){
                        c.add_Hardware(h);
                    }
                }

                while(c.getSize()<3){
                        rd = getRand(HW_vec.size());
                        c.add_Hardware(HW_vec[rd]);
                }
                ch_vec.push_back( c );
            }
            
            for(Hardware h : HW_vec){
                bool All_connected = false;
                for(COM c : ch_vec){
                    if(c.isConnected(h.getID())){
                        All_connected = true;
                    }
                }
                if(All_connected == false){
                        ch_vec[0].add_Hardware(h);
                    }
            }

}
