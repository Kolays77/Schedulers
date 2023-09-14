#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

#include "task.h"
#include "stat.h"

const int QUANTUM = 1;  // 1ms

int gcd(int a, int b);
int lcm(const std::vector<Task>& tasks);

struct Scheduler {
    std::vector<Task> tasks;
    int hyperperiod;
    std::priority_queue<Task> priorityQueue;
    Scheduler(){};
    Scheduler(const std::vector<Task>& tasks);
    Scheduler(std::vector<Task>&& tasks);
};


SchedulerStat EDF(Scheduler&& scheduler, std::ofstream&& outputFile);
SchedulerStat RoundRobin(Scheduler&& scheduler, std::ofstream&& outputFile);
SchedulerStat FIFO(Scheduler&& scheduler, std::ofstream&& outputFile);


// TODO посчитать процент загруженности CPU (utilization)
// TODO добавить FIFO
// TODO добавить время простоя
// TODO добавить время появление на графике
// TODO статистика по каждой задачи

// 2) Сколько в среднем тратиться времени на выполнение