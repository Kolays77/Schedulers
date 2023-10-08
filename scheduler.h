#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include "task.h"

// TODO посчитать процент загруженности CPU (utilization)

const uint32_t IDLE = 0;

enum class MODE {
  RANDOM, SAMPLE
};

enum class ALGO {
  EDF, ROUND_ROBIN, SJF
};

int countHyperperiod(const std::vector<Task> &tasks) {
  int res = 1;
  for (int i = 0; i < tasks.size(); ++i) {
    res = std::lcm(res, tasks[i].period);
  }
  return res;
}

std::vector<Task> loadSample() {
    return {
      Task{
        .id = 1,
        .arrival = 0,
        .period = 9,
        .deadline = 6,
        .execution = 3,
        .priority = 1
      },
      Task{
        .id = 2,
        .arrival = 0,
        .period = 12,
        .deadline = 5,
        .execution = 2,
        .priority = 1,
      },
      Task{
        .id = 3,
        .arrival = 0,
        .period = 16,
        .deadline = 10,
        .execution = 4,
        .priority = 1,
    }
  };
}

std::vector<Task> generateRandomTasks(int n, int maxPeriod, int maxExecution,
                                      int maxDeadline) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> executionTimeDist(1, maxExecution);
  std::vector<Task> generatedTasks;
  for (int i = 0; i < n; ++i) {
    int id = i+1;
    int executionTime = executionTimeDist(gen);
    std::uniform_int_distribution<> periodDist(executionTime+5, maxPeriod);
    int period = periodDist(gen);
    std::uniform_int_distribution<> deadlineDist(period+3, maxDeadline);
    int arrival = 0;
    int deadline = deadlineDist(gen);
    generatedTasks.push_back(Task{.id = id,
                                  .arrival = arrival,
                                  .period = period,
                                  .deadline = deadline,
                                  .execution = executionTime,
                                  .const_execution = executionTime,
                                  .priority = 1});
  }
  return generatedTasks;
}

std::vector<Task> generateTasks(MODE mode, int n=5, int maxPeriod=15, int maxExecutionTime=3, int maxDeadline=20) {
  if (mode == MODE::RANDOM) {
    return generateRandomTasks(n, maxPeriod, maxExecutionTime, maxDeadline);
  } else {
    return loadSample();
  }
}

template <typename C>
struct Container {
  void Add();
  int Size();
  void Empty();
  Task Get();
};


struct CompareEDF{
  bool operator()(const Task &lhs, const Task &rhs) {
    return (lhs.addedTime + lhs.deadline) < (rhs.addedTime + rhs.deadline);
  }
};


struct CompareSJF{
  bool operator()(Task lhs, Task rhs) {
    return lhs.const_execution > rhs.const_execution;
  }
};

template <>
struct Container<std::priority_queue<Task, std::vector<Task>, CompareEDF> > {
  void Add(Task task) {
    data.push(task);
  }
  int Size() {
    return data.size();
  }
  bool Empty() {
    return data.empty();
  }
  Task Get() {
    Task top = data.top();
    data.pop();
    return top;
  }
private:
  std::priority_queue<Task, std::vector<Task>, CompareEDF> data;
};

template <>
struct Container<std::priority_queue<Task, std::vector<Task>, CompareSJF> > {
  void Add(Task task) {
    data.push(task);
  }
  int Size() {
    return data.size();
  }
  bool Empty() {
    return data.empty();
  }
  Task Get() {
    Task top = data.top();
    data.pop();
    return top;
  }
private:
  std::priority_queue<Task, std::vector<Task>, CompareSJF> data;
};


template <>
struct Container<std::queue<Task> > {
  void Add(Task task) {
    data.push(task);
  }
  int Size() {
    return data.size();
  }
  bool Empty() {
    return data.empty();
  }
  Task Get() {
    Task top = data.front();
    data.pop();
    return top;
  }
private:
  std::queue<Task> data;
};


struct Scheduler {
  Scheduler(const std::vector<Task>& tasks, ALGO algo=ALGO::EDF) :
    tasks(tasks),
    cpuUtilization(CountUtilizationOfCPU()),
    completedTasks(std::vector<int>(tasks.size()+1, 0)),
    hyperperiod(countHyperperiod(tasks)), 
    algorithm(algo),
    trace(std::vector<int>(hyperperiod)),
    queueCount(std::vector<int>(hyperperiod)), 
    totalFailedDeadlineByTask(std::vector<int>(tasks.size(), 0)) {}
  
  std::vector<Task> tasks;
  int hyperperiod;
  void PrintStat() const;

  const double cpuUtilization;
  double CountUtilizationOfCPU();
  // Statistics
  std::vector<int> completedTasks;
  std::vector<int> queueCount;
  std::vector<int> trace;
  std::vector<int> totalFailedDeadlineByTask;
  ALGO algorithm;
};


template< typename C>
void Schedule(Scheduler& schedeler) {
  if (schedeler.tasks.empty()) {
    return;
  }

  Container<C> container;
  const size_t numOfTasks = schedeler.tasks.size();
  for (int tick = 0; tick < schedeler.hyperperiod; tick += 1) {
    for (const Task &task : schedeler.tasks) {
      if (tick == task.arrival ||
          (tick && ((tick + task.arrival) % task.period == 0))) {
        Task addedTask = task;
        addedTask.addedTime = tick;
        container.Add(addedTask);
      }
    }
    
    schedeler.queueCount[tick] = container.Size();
    if (container.Empty()) {
      schedeler.completedTasks[IDLE] += 1;
      schedeler.trace[tick] = IDLE;
      continue;
    }

    const Task& activeTask = container.Get();
    activeTask.Execute();
    schedeler.trace[tick] = activeTask.id;
    if (activeTask.execution == 0) {
      schedeler.completedTasks[activeTask.id] += 1;
      
      // Подсчет количества задач для не выполненных к назначеному времени
      if (tick > activeTask.deadline + activeTask.addedTime) {
        schedeler.totalFailedDeadlineByTask[activeTask.id-1] += 1;
      }
    } else {
      container.Add(activeTask);
    }
  }

  // Подсчет статистики для "хвоста" - не выполненных к назначеному времени
  while (!container.Empty()) {
    Task top = container.Get();
    if (schedeler.hyperperiod > top.deadline + top.addedTime) {
        schedeler.totalFailedDeadlineByTask[top.id-1] += 1;
      }
  }
}

void Run(Scheduler& scheduler) {
  if (scheduler.algorithm == ALGO::EDF) {
    Schedule<std::priority_queue<Task, std::vector<Task>, CompareEDF> >(scheduler);
  } else if (scheduler.algorithm == ALGO::SJF) {
    Schedule<std::priority_queue<Task, std::vector<Task>, CompareSJF> >(scheduler);
  } else {
    Schedule<std::queue<Task> >(scheduler);
  }
}

double Scheduler::CountUtilizationOfCPU() {
  return 1.0;
}

void Scheduler::PrintStat() const {
  const size_t numOfTasks = tasks.size();
  if (numOfTasks <= 1) {
    return;
  }
  int totalCompleted = 0;
  int totalFailed = 0;

  std::string tracePath;
  std::string queueSizePath;

  if (algorithm == ALGO::EDF) {
    std::cout << "EDF\n";
    tracePath = "out/trace_edf.txt";
    queueSizePath = "out/queue_size_edf.txt";
  } else if (algorithm == ALGO::SJF) {
    std::cout << "SJF\n";
    tracePath = "out/trace_sjf.txt";
    queueSizePath = "out/queue_size_sjf.txt";
  } else {
    std::cout << "RoundRobin\n";
    tracePath = "out/trace_round_robin.txt";
    queueSizePath = "out/queue_size_round_robin.txt";
  }
  std::ofstream outTrace(tracePath);
  std::ofstream outQueueSize(queueSizePath);

  for (int tick = 0; tick < hyperperiod; ++tick) {
    outTrace << tick << " " << trace[tick] << "\n";
    outQueueSize << tick << " " << queueCount[tick] << "\n";
  }

  std::cout << "hyperperiod " << hyperperiod << "\n";
  std::cout << "IDLE times : " << completedTasks[IDLE] << "\n";
  for (size_t i = 1; i <= numOfTasks ; ++i) {
    std::cout << i << " task : " << completedTasks[i] << " " << totalFailedDeadlineByTask[i-1] << "\n";
    totalCompleted += completedTasks[i];
    totalFailed += totalFailedDeadlineByTask[i-1];
  }
  
  std::cout << "TOTAL COMPLETED " << totalCompleted << "\n";
  std::cout << "TOTAL FAILED DEADLINE " << totalFailed << "\n";

}