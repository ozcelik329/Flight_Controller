#include "Scheduler.h"

SchedulerTaskConfig Scheduler::_tasks[8] = {};
uint8_t Scheduler::_taskCount = 0;

void Scheduler::init() {
    _taskCount = 0;
}

void Scheduler::registerTask(const char* name, uint32_t periodMs) {
    if (_taskCount < 8) {
        _tasks[_taskCount].name = name;
        _tasks[_taskCount].periodMs = periodMs;
        _tasks[_taskCount].lastRunMs = 0;
        _taskCount++;
    }
}

void Scheduler::update() {
    uint32_t now = millis();
    for (uint8_t i = 0; i < _taskCount; ++i) {
        if (now - _tasks[i].lastRunMs >= _tasks[i].periodMs) {
            _tasks[i].lastRunMs = now;
        }
    }
}
