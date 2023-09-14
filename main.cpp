#include "src/scheduler.h"
#include "src/generate.h"

void simpleTest() {
    {
        Task t1 { 
            .id=1, 
            .arrival=0,
            .period=9, 
            .deadline=6,
            .execution=3,
            .priority=1
        };
        
        Task t2 { 
            .id=2, 
            .arrival=0,
            .period=12, 
            .deadline=5,
            .execution=2,
            .priority=1,
        };
        
        Task t3 { 
            .id=3,
            .arrival=0,
            .period=16, 
            .deadline=10,
            .execution=4,
            .priority=1,
        };
        auto stat1 = EDF(Scheduler{{t1,t2,t3}}, std::ofstream{"out/output_1_edf.txt"}); // TODO make ifexist out/
        auto stat2 = RoundRobin(Scheduler{{t1,t2,t3}}, std::ofstream{"out/output_1_rr.txt"});

        stat1.Print();
        stat2.Print();
    }
}

int main() {
    const auto tasks = generateRandomTasks(10, 10, 5, 20);
    EDF(Scheduler(tasks), std::ofstream{"out/output_1_edf.txt"}).Print(); // TODO make ifexist out/
    RoundRobin(Scheduler(tasks), std::ofstream{"out/output_1_rr.txt"}).Print();
    return 0;
}