#include "scheduler.h"

#include "stat.h"
#include "task.h"
#include <algorithm>
#include <numeric>
#include <queue>
#include <vector>

int countHyperperiod(const std::vector<Task> &tasks) {
  int res = 1;
  for (int i = 0; i < tasks.size(); ++i) {
    res = std::lcm(res, tasks[i].period);
  }
  return res;
}

Scheduler::Scheduler(const std::vector<Task> &tasks)
    : tasks(tasks), hyperperiod(countHyperperiod(tasks)){};

// Scheduler::Scheduler(std::vector <Task>&& tasks) :
//     tasks(tasks),
//     hyperperiod(lcm(tasks)) {};

// Earliest Deadline first algorithm
SchedulerStat EDF(const Scheduler &scheduler, std::ofstream &&outputFile) {
  std::vector<int> idleTime;
  const size_t numOfTasks = scheduler.tasks.size();
  SchedulerStat statResult(numOfTasks);
  std::priority_queue<Task> priorityQueue;
  for (int tick = 0; tick < scheduler.hyperperiod; tick += QUANTUM) {

    for (const Task &task : scheduler.tasks) {
      if (tick == task.arrival ||
          (tick && ((tick + task.arrival) % task.period == 0))) {
        Task addedTask = task; // modified copy
        addedTask.addedTime = tick;
        priorityQueue.push(addedTask); // sort by deadline, curr_time + deadline
      }
    }

    if (priorityQueue.empty()) {
      idleTime.push_back(tick);
      outputFile << tick << "  IDLE"
                 << "\n";
      statResult.completedTasks[numOfTasks] += 1;
      continue;
    }

    const Task &activeTask = priorityQueue.top();
    activeTask.Execute();

    outputFile << tick << "  Task   " << activeTask.id << "\n";
    if (activeTask.execution == 0) {
      statResult.completedTasks[activeTask.id - 1] += 1;
      priorityQueue.pop();
    }
  }
  statResult.hyperperiod = scheduler.hyperperiod;
  return statResult;
}

// TODO refactor
SchedulerStat RoundRobin(const Scheduler &scheduler,
                         std::ofstream &&outputFile) {
  std::vector<int> idleTime;
  std::queue<Task> queue;
  const size_t numOfTasks = scheduler.tasks.size();
  SchedulerStat statResult(numOfTasks);

  for (int tick = 0; tick < scheduler.hyperperiod; tick += QUANTUM) {

    for (const Task &task : scheduler.tasks) {
      if (tick == task.arrival ||
          (tick && ((tick + task.arrival) % task.period == 0))) {
        Task addedTask = task;
        addedTask.addedTime = tick;
        queue.push(addedTask);
      }
    }

    if (queue.empty()) {
      idleTime.push_back(tick);
      outputFile << tick << "  IDLE"
                 << "\n";
      statResult.completedTasks[numOfTasks] += 1;
      continue;
    }

    const Task &activeTask = queue.front();
    activeTask.Execute();
    outputFile << tick << "  Task   " << activeTask.id << "\n";
    if (activeTask.execution != 0) {
      queue.push(activeTask);
      statResult.completedTasks[activeTask.id - 1] += 1;
    }
    queue.pop();
  }
  statResult.hyperperiod = scheduler.hyperperiod;
  return statResult;
}