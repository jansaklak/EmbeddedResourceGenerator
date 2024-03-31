#ifndef TIMEANDCOST_H
#define TIMEANDCOST_H

class TimeAndCost {
private:
    int cost;
    int time;

public:
    TimeAndCost(int _Cost, int _time);
    int getCost() const;
    int getTime() const;
};

#endif // TIMEANDCOST_H
