#include "scheduler.h"

bool Task::operator < (const Task & other) const {
    return addedTime + deadline > other.addedTime + other.deadline;
}

void Task::Execute() const {
    execution--;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

int lcm(const std::vector < Task > & tasks) {
    int result = tasks[0].period;
    for (int i = 1; i < tasks.size(); ++i) {
        result = (result * tasks[i].period) / gcd(result, tasks[i].period);
    }
    return result;
}

Scheduler::Scheduler(const std::vector < Task > & tasks) : 
    tasks(tasks),
    hyperperiod(lcm(tasks)),
    completedTasks(std::vector < int > (tasks.size(), 0)) {};

Scheduler::Scheduler(std::vector <Task>&& tasks) : 
    tasks(tasks),
    hyperperiod(lcm(tasks)),
    completedTasks(std::vector<int>(tasks.size(), 0)) {};

    //Earliest Deadline first algorithm
void EDF(Scheduler && scheduler, std::ofstream && outputFile) {
    std::vector < int > idleTime;
    for (int tick = 0; tick < scheduler.hyperperiod; tick += QUANTUM) {
        
        for (const Task & task: scheduler.tasks) {
            if (tick == task.arrival || (tick && ((tick + task.arrival) % task.period == 0))) {
                Task addedTask = task; // modified copy
                addedTask.addedTime = tick;
                scheduler.priorityQueue.push(addedTask); // sort by deadline, curr_time + deadline
            }
        }
        
        if (scheduler.priorityQueue.empty()) {
            idleTime.push_back(tick);
            outputFile << tick << "  IDLE" << "\n";
            continue;
        }
        const Task & activeTask = scheduler.priorityQueue.top();

        activeTask.Execute();
        outputFile << tick << "  Task   " << activeTask.id << "\n";
        if (activeTask.execution == 0) {
            scheduler.priorityQueue.pop();
        }
    }
}

//TODO refactor
void RoundRobin(Scheduler && scheduler, std::ofstream && outputFile) {
    std::vector < int > idleTime;
    std::queue < Task > queue;

    for (int tick = 0; tick < scheduler.hyperperiod; tick += QUANTUM) {

        for (const Task & task: scheduler.tasks) {
            if (tick == task.arrival || (tick && ((tick + task.arrival) % task.period == 0))) {
                Task addedTask = task;
                addedTask.addedTime = tick;
                queue.push(addedTask);
            }
        }

        if (queue.empty()) {
            idleTime.push_back(tick);
            outputFile << tick << "  IDLE" << "\n";
            continue;
        }

        const Task & activeTask = queue.front();
        activeTask.Execute();
        outputFile << tick << "  Task   " << activeTask.id << "\n";
        if (activeTask.execution != 0) {
            queue.push(activeTask);
        }
        queue.pop();
    }
}