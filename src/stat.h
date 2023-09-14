#pragma once
#include <vector>
#include "task.h"

struct SchedulerStat {
    SchedulerStat(int numOfTasks);
    void Print() const;
    int CountUtilizationOfCPU();
    
    std::vector<int> completedTasks;
    double cpuUtilization;
};