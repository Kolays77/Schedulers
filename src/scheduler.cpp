#include "scheduler.h"

#include <vector>
#include "stat.h"
#include "task.h"

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
    hyperperiod(lcm(tasks)) {};

Scheduler::Scheduler(std::vector <Task>&& tasks) : 
    tasks(tasks),
    hyperperiod(lcm(tasks)) {};



    //Earliest Deadline first algorithm
SchedulerStat EDF(Scheduler && scheduler, std::ofstream && outputFile) {
    std::vector < int > idleTime;
    const size_t numOfTasks = scheduler.tasks.size();
    SchedulerStat statResult(numOfTasks);
    
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
            statResult.completedTasks[numOfTasks] += 1;
            continue;
        }
        const Task & activeTask = scheduler.priorityQueue.top();

        activeTask.Execute();
        outputFile << tick << "  Task   " << activeTask.id << "\n";
        if (activeTask.execution == 0) {
            scheduler.priorityQueue.pop();
            statResult.completedTasks[activeTask.id-1] += 1;
        }
    }
    return statResult;
}


//TODO refactor
SchedulerStat RoundRobin(Scheduler && scheduler, std::ofstream && outputFile) {
    std::vector < int > idleTime;
    std::queue < Task > queue;
    const size_t numOfTasks = scheduler.tasks.size();
    SchedulerStat statResult(numOfTasks); 

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
            statResult.completedTasks[numOfTasks] += 1;
            continue;
        }

        const Task & activeTask = queue.front();
        activeTask.Execute();
        outputFile << tick << "  Task   " << activeTask.id << "\n";
        if (activeTask.execution != 0) {
            queue.push(activeTask);
            statResult.completedTasks[activeTask.id-1] += 1;
        }
        queue.pop();
    }

    return statResult;
}