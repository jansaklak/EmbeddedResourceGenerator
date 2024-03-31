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
void COM::add_Hardware(const Hardware* h) {
    if(isConnected(h)){
        return;
    }
    HW_line.push_back(*h);
}

int COM::getSize() const{
    return HW_line.size();
}
int COM::getBandwidth() const{
    return bandwidth;
}
int COM::getCost() const{
    return connect_cost;
}
int COM::getID() const{
    return id;
}

bool COM::operator<(const COM& other) const {
        return id < other.id;
}

void COM::PrintCOM(int hw_size,std::ostream& out) const{
    out << "CHAN" << id << " " << bandwidth << " " << connect_cost << " ";
            for (Hardware h : HW_line){
                out << isConnected(&h) << " ";
            }
    out << "\n";
}

bool COM::isConnected(const Hardware* other) const{
    for(Hardware h : HW_line){
        if(h.getID() == other->getID()) return true;
    }
    return false;
}
