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

void COM::remove_Hardware(const Hardware* h) {
    for (auto it = HW_line.begin(); it != HW_line.end(); ++it) {
        if ((*it).getID() == h->getID()) {
            HW_line.erase(it);
            break;
        }
    }
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

void COM::printCOM(std::ostream& out,std::vector<Hardware> hws) const{
    out << "CHAN" << id << " " << bandwidth << " " << connect_cost << " ";
            for (Hardware h : hws){
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