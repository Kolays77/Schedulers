#pragma once
#include "task.h"
#include <vector>

struct SchedulerStat {
  SchedulerStat(int numOfTasks);
  void Print() const;
  int CountUtilizationOfCPU();

  std::vector<int> completedTasks;
  double cpuUtilization;
  int hyperperiod;
};