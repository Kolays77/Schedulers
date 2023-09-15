#include "task.h"

bool operator<(const Task &lhs, const Task &rhs) {
  return lhs.addedTime + lhs.deadline > rhs.addedTime + rhs.deadline;
}

void Task::Execute() const { execution--; }
