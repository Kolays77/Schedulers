#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <vector>
#include "task.h"

#include "scheduler.h"

int main() {

  const auto tasks = generateTasks(MODE::RANDOM);
  for (int i = 0; i < tasks.size(); ++i) {
    std::cout << "id " << i+1 
              << " t_arr : "  << tasks[i].arrival 
              << " t_per : "  << tasks[i].period 
              << " t_dead : " << tasks[i].deadline
              << " t_exec : " << tasks[i].execution << "\n";
  }

  {
    Scheduler sch(tasks, ALGO::EDF);
    Run(sch);
    sch.PrintStat();
  }


  std::cout << "=================================\n";

  {
    Scheduler sch(tasks, ALGO::ROUND_ROBIN);
    Run(sch);
    sch.PrintStat();
  }

  std::cout << "=================================\n";

  {
    Scheduler sch(tasks, ALGO::SJF);
    Run(sch);
    sch.PrintStat();
  }

  return 0;
}