#pragma once
#include "task.h"
#include <vector>

std::vector<Task> generateRandomTasks(int n, int maxPeriod,
                                      int maxExecutionTime, int maxDeadline);
