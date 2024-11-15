// Arduino Frequency Counter
// 19.10.2021
// Stefan Rau
// Provides a task manager for time based task handling

#pragma once
#ifndef _TaskHandler_h
#define _TaskHandler_h

#include "List.h"
#include "Debug.h"

// #if DEBUG_APPLICATION > 0
// #define TIMER_INTERRUPT_DEBUG 3
// #define _TIMERINTERRUPT_LOGLEVEL_ 3
// #define LOCAL_DEBUG 0
// #include "TimerInterrupt_Generic_Debug.h"
// #else
// #define TIMER_INTERRUPT_DEBUG 0
// #define _TIMERINTERRUPT_LOGLEVEL_ 0
// #define LOCAL_DEBUG 0
// #endif

void TaskDispatcher();

/// <summary>
/// Class that contains a single list task.
/// </summary>
class Task
{
public:
    enum class eTaskType : char
    {
        TOneTime = 'O',         // This task runs only once
        TCyclic = 'C',          // This task runs ever and ever again
        TFollowUpOneTime = 'o', // This task runs after ending a well defined other task once
        TFollowUpCyclic = 'c',  // This task runs after ending a well defined other task ever and ever again
        TTriggerOneTime = 'T'   // Task runs a defined time after a trigger is recognized
    };

    static Task *GetNewTask(eTaskType iTaskType, int iTicks, void (*iCallback)(void));

    /// <summary>
    /// Central task loop triggered by a cyclic timer event
    /// </summary>
    void Process();

    /// <summary>
    /// The given task must be done before this task can start
    /// </summary>
    /// <param name="iPreviouslyProcessed">Given task</param>
    void DefinePrevious(Task *iPreviouslyProcessed);

    /// <summary>
    /// Starts a task, if it's not running already
    /// </summary>
    void Start();

    /// <summary>
    /// Starts a task, although it's already running
    /// </summary>
    void Restart();

private:
    enum class eTaskState : char
    {
        TWaiting = 'W', // the current task is waiting for being processed
        TRunning = 'R', // the current task is currently running
        TDone = 'D'     // the current task has ended
    };

    volatile eTaskState mTaskState; // current state of the task
    volatile eTaskType mTaskType;   // type of the current task
    volatile int mTaskCounter;      // internal counter for ticks (starts with _mTicks and then counts down to 0)
    volatile int mTicks;            // number of internal timer cycles

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="iTaskType">Kind of task</param>
    /// <param name="iTicks">Number of ticks relevant for that task - depending on iTaskType, it can control the cycle time or time until a task starts</param>
    /// <param name="iCallback">Address of the function implementing the task handler</param>
    Task(eTaskType iTaskType, int iTicks, void (*iCallback)());
    ~Task();

    volatile Task *mPreviouslyProcessed = nullptr; // Previous task
    void (*mCallback)();                           // Address of the function implementing the task handler
};

/// <summary>
/// Handles processing of all tasks.
/// </summary>
class TaskHandler
{
public:
    /// <summary>
    /// Factory for managing single instances
    /// </summary>
    /// <returns>Instance of the task handler</returns>
    static TaskHandler *GetInstance();

    /// <summary>
    /// Initialize hardware timer depending on hardware
    /// </summary>
    /// <param name="iCycleTimeInMs">Set Cycle time of the timer in milliseconds</param>
    void SetCycleTimeInMs(unsigned long iCycleTimeInMs);

    /// <summary>
    /// Get a list of all tasks
    /// </summary>
    /// <returns>Instance of object collection</returns>
    ListCollection *GetTaskList();

private:
    ListCollection *mTaskList; // List object containing the tasks

    /// <summary>
    /// Constructor
    /// </summary>
    TaskHandler();
    ~TaskHandler();
};

#endif
