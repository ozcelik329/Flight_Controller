#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

struct SchedulerTaskConfig {
    const char* name;
    uint32_t periodMs;
    uint32_t lastRunMs;
};

class Scheduler {
  public:
    static void init();
    static void registerTask(const char* name, uint32_t periodMs);
    static void update();

  private:
    static SchedulerTaskConfig _tasks[8];
    static uint8_t _taskCount;
};

#endif
