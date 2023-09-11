#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

const int QUANTUM = 1;  // 1ms

struct Task {
    int id = 0;
    int arrival = 0;
    int period = 0;
    int deadline = 0;
    mutable int execution = 0;
    int priority = 0;
    int addedTime = 0;

    inline bool operator<(const Task& other) const;
    inline void Execute() const;
};

int gcd(int a, int b);
int lcm(const std::vector<Task>& tasks);


struct Scheduler {
    std::vector<Task> tasks;
    int hyperperiod;
    std::priority_queue<Task> priorityQueue;
    std::vector<int> completedTasks;

    Scheduler(){};
    Scheduler(const std::vector<Task>& tasks);
    Scheduler(std::vector<Task>&& tasks);
};

// TODO посчитать процент загруженности CPU (utilization)
// TODO добавить FIFO

void EDF(Scheduler&& scheduler, std::ofstream&& outputFile);
void RoundRobin(Scheduler&& scheduler, std::ofstream&& outputFile);