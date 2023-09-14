#include "stat.h"
#include <iostream>

SchedulerStat::SchedulerStat(int numOfTasks) : 
    completedTasks(std::vector<int>(numOfTasks+1, 0)), 
    cpuUtilization(0) {};

void SchedulerStat::Print() const {
    const size_t numOfTasks = completedTasks.size();
    std::cout << "hyperperiod " << hyperperiod << "\n";
    if (numOfTasks == 1) { 
        return;
    }
    for (size_t i = 0; i < numOfTasks - 1; ++i) {
        std::cout << i + 1 << " task : " << completedTasks[i] << "\n";
    }
    std::cout << "IDLE times : " << completedTasks[numOfTasks-1] << "\n";
}

int SchedulerStat::CountUtilizationOfCPU() {
    cpuUtilization = 0;
    return 0;
}