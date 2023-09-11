#include "scheduler.h"

int main() {
    {
        Task t1 { 
            .id=1, 
            .arrival=1,
            .period=9, 
            .deadline=6,
            .execution=3,
            .priority=1
        };
        
        Task t2 { 
            .id=2, 
            .arrival=3,
            .period=12, 
            .deadline=5,
            .execution=2,
            .priority=1,
        };
        
        Task t3 { 
            .id=3,
            .arrival=4,
            .period=16, 
            .deadline=10,
            .execution=4,
            .priority=1,
        };
        EDF(Scheduler{{t1,t2,t3}}, std::ofstream{"out/output_1_edf.txt"}); // TODO make ifexist out/
        RoundRobin(Scheduler{{t1,t2,t3}}, std::ofstream{"out/output_1_rr.txt"});
    }

    {
        Task t1 { 
            .id=1, 
            .arrival=0,
            .period=20,
            .deadline=7,
            .execution=3,
            .priority=1
        };
        
        Task t2 { 
            .id=2, 
            .arrival=0,
            .period=5, 
            .deadline=4,
            .execution=2,
            .priority=1,
        };
        
        Task t3 { 
            .id=3,
            .arrival=0,
            .period=10, 
            .deadline=8,
            .execution=2,
            .priority=1,
        };

        EDF(Scheduler{{t1,t2,t3}}, std::ofstream{"out/output_1_edf.txt"}); // TODO make ifexist out/
        RoundRobin(Scheduler{{t1,t2,t3}}, std::ofstream{"out/output_1_rr.txt"});
    }
    
    return 0;
}