#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <random>

#include "generate.h"

std::vector<Task> generateRandomTasks(int n, int maxPeriod, int maxExecution, int maxDeadline) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> periodDist(1, maxPeriod);
    std::uniform_int_distribution<> executionTimeDist(1, maxExecution);
    std::uniform_int_distribution<> deadlineDist(maxExecution, maxDeadline);
    std::vector<Task> generatedTasks;
    for (int i = 0; i < n; ++i) {
        int id = i;
        int period = periodDist(gen);
        int arrival = 0;
        int executionTime = executionTimeDist(gen);
        int deadline = deadlineDist(gen);
        std::cout << "id " << id << " per = " << period << " exec " << executionTime << " deadline " << deadline << "\n";
        
        generatedTasks.push_back(Task{ 
                                    .id=id, 
                                    .arrival=arrival,
                                    .period=period, 
                                    .deadline=deadline,
                                    .execution=executionTime,
                                    .priority=1}
                                );
    }
    return generatedTasks;
}

// TODO проверить, когда период меньше exec стату
// id 0 per = 2 exec 5 deadline 15
// id 1 per = 3 exec 5 deadline 13