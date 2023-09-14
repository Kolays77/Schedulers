// #include <iostream>
// #include <fstream>
// #include <queue>
// #include <vector>
// #include <random>

// #include "task.h"

// std::vector<Task> generateRandomTasks(int n, int maxPeriod, int maxexecutionTime) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> periodDist(1, maxPeriod);
//     std::uniform_int_distribution<> executionTimeDist(1, maxexecutionTime);
//     std::uniform_int_distribution<> priorityDist(1, 5);
//     std::vector<Task> generatedTasks;
//     for (int i = 0; i < n; ++i) {
//         int id = i + 1;
//         int period = 10;
//         int executionTime = executionTimeDist(gen);
//         int priority = priorityDist(gen);
//         std::cout << "id " << id << " per = " << period << " exec " << executionTime << " prior " << priority << "\n";
//         // TODO fix
//         generatedTasks.push_back(Task(id, period, executionTime, priority));
//     }

//     return generatedTasks;
// }