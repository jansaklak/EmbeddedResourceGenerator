#include "COM.h"

COM::COM(int _bandwidth,int _cost,int _id) {
    bandwidth = _bandwidth;
    connect_cost = _cost;
    id = _id;
}
COM::COM(std::vector<Hardware> _HW_line, int _bandwidth,int _cost,int _id) {
    HW_line = _HW_line;
    bandwidth = _bandwidth;
    connect_cost = _cost;
    id = _id;
}
void COM::add_Hardware(Hardware h) {
    if(isConnected(h.getID())){
        return;
    }
    HW_line.push_back(h);
}
int COM::getSize() {
    return HW_line.size();
}
int COM::getBandwidth() {
    return bandwidth;
}
int COM::getCost() {
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

bool COM::isConnected(int _id) {
    for(Hardware h : HW_line){
        if(h.getID() == _id) return true;
    }
    return false;
}

bool COM::isConnected(Hardware other) {
    for(Hardware h : HW_line){
        if(h.getID() == other.getID()) return true;
    }
    return false;
}
