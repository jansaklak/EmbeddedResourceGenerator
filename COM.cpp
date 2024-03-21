#include "COM.h"

COM::COM(int bd,int cost,int set_id) {
    bandwidth = bd;
    connect_cost = cost;
    id = set_id;
}
COM::COM(std::vector<Hardware> h_list, int bd,int cost,int set_id) {
    line = h_list;
    bandwidth = bd;
    connect_cost = cost;
    id = set_id;
}
void COM::add_Hardware(Hardware h) {
    if(isConnected(h.getID())){
        return;
    }
    line.push_back(h);
}
int COM::getSize() {
    return line.size();
}
int COM::getB() {
    return bandwidth;
}
int COM::getC() {
    return connect_cost;
}
int COM::getID() {
    return id;
}

bool COM::operator<(const COM& other) const {
        return id < other.id;
}

void COM::PrintCOM(int hw_size,std::ostream& out) {
    out << "CHAN" << id << " " << bandwidth << " " << connect_cost << " ";
            for (int i = 0;i<hw_size;i++){
                out << isConnected(i) << " ";
            }
    out << "\n";
}

bool COM::isConnected(int find_id) {
    for(Hardware h : line){
        if(h.getID() == find_id) return true;
    }
    return false;
}
