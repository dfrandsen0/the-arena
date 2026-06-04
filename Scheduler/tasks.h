#ifndef SCHEDULER_TASKS_H_
#define SCHEDULER_TASKS_H_

#include <thread>
#include <atomic>

#include "scheduler.h"
#include "..\Graphics\graphics.h"
#include "..\Mechanics\userState.h"

class Tasks {
public:
    static void RunStartingTasks(SchedulerModule* schedulerModule);
    static void RunMainPrepTasks(SchedulerModule* schedulerModule);
    static std::atomic<bool> done;
private:
    Tasks();

    static void DrawStartingButtons(GraphicsModule* graphicsModule, UserState* userState);
};

#endif