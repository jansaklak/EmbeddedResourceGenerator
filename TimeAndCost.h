#ifndef TIMEANDCOST_H
#define TIMEANDCOST_H

class TimeAndCost {
private:
    int cost;
    int time;

public:
    TimeAndCost(int _Cost, int _time);
    int getCost();
    int getTime();
};

#endif // TIMEANDCOST_H
