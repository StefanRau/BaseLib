// Arduino Base Libs
// 18.10.2021
// Stefan Rau
// History
// 18.10.2021: 1st version - Stefan Rau
// 12.01.2022: extended by ARDUINO_NANO_RP2040_CONNECT - Stefan Rau
// 16.03.2022: ARDUINO_NANO_RP2040_CONNECT removed - Stefan Rau
// 20.06.2022: Debug instantiation of classes - Stefan Rau
// 08.08.2022: Switch to ARDUINO NANO IOT due to memory issues - Stefan Rau
// 21.09.2022: use GetInstance instead of Get<Typename> - Stefan Rau
// 26.09.2022: DEBUG_APPLICATION defined in platform.ini - Stefan Rau
// 21.12.2022: extend destructor - Stefan Rau
// 28.12.2022: fix bug with timer interrupt for NANO 33 IOT - Stefan Rau
// 16.07.2023: Use new capabilities of list processing - Stefan Rau

#include "TaskHandler.h"
#include <Arduino.h>

#ifdef ARDUINO_AVR_NANO_EVERY
#define USING_16MHZ true
#define USING_8MHZ false
#define USING_250KHZ false
#define USE_TIMER_0 false
#define USE_TIMER_1 true
#define USE_TIMER_2 false
#define USE_TIMER_3 false
#endif

#ifdef ARDUINO_SAMD_NANO_33_IOT
#define USING_TIMER_TC3 true
#define USING_TIMER_TC4 false
#define USING_TIMER_TC5 false
#define USING_TIMER_TCC false
#define USING_TIMER_TCC1 false
#define USING_TIMER_TCC2 false
#endif

#include <TimerInterrupt_Generic.h>

#if defined(ARDUINO_AVR_NANO_EVERY)
#define TIMER1_TICKS_FOR_1_MS 1
#define lTimer ITimer1
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
#define TIMER1_TICKS_FOR_1_MS 1000
static SAMDTimer lTimer(TIMER_TC3);
#elif defined(ARDUINO_ARDUINO_NANO33BLE)
#define TIMER1_TICKS_FOR_1_MS 10
static NRF52_MBED_Timer lTimer(NRF_TIMER_3);
#define TASK_HANDLER_TIMER_DEFINED
#else
#error Task Handler not defined for given processor
#endif

static TaskHandler *gInstance = nullptr;

void TaskDispatcher()
{
	Task *lTask;

	for (ListElement *lTaskIterator = TaskHandler::GetInstance()->GetTaskList()->IterateStart(); lTaskIterator != nullptr;)
	{
		// Ping each single task
		Task *lTask = (Task *)TaskHandler::GetInstance()->GetTaskList()->Iterate(&lTaskIterator);
		lTask->Process();
	}
}

/////////////////////////////////////////////////////////////

TaskHandler::TaskHandler()
{
	DEBUG_INSTANTIATION("TaskHandler");

	mTaskList = ListCollection::GetInstance();

	// Initialize timer
#ifdef ARDUINO_AVR_NANO_EVERY
	lTimer.init();
#endif
}

TaskHandler::~TaskHandler()
{
	DEBUG_DESTROY("TaskHandler");
	delete mTaskList;
	// delete lTimer;
}

TaskHandler *TaskHandler::GetInstance()
{
	DEBUG_METHOD_CALL("TaskHandler::GetInstance");

	// Returns a pointer to singleton instance
	gInstance = (gInstance == nullptr) ? new TaskHandler : gInstance;
	return gInstance;
}

void TaskHandler::SetCycleTimeInMs(unsigned long iCycleTimeInMs)
{
	DEBUG_METHOD_CALL("TaskHandler::SetCycleTimeInMs");

	// Initialize hardware timer
#ifdef ARDUINO_SAMD_NANO_33_IOT
	if (lTimer.attachInterruptInterval((float)iCycleTimeInMs * TIMER1_TICKS_FOR_1_MS, TaskDispatcher))
#endif
#if defined(ARDUINO_ARDUINO_NANO33BLE) or defined(ARDUINO_AVR_NANO_EVERY)
		if (lTimer.attachInterruptInterval(iCycleTimeInMs * TIMER1_TICKS_FOR_1_MS, TaskDispatcher))
#endif
		{
			DEBUG_PRINT_LN("Task timer set");
		}
		else
		{
			DEBUG_PRINT_LN("Setting Task timer failed");
		};
}

ListCollection *TaskHandler::GetTaskList()
{
	return mTaskList;
}

/////////////////////////////////////////////////////////////

Task::Task(Task::eTaskType iTaskType, int iTicks, void (*iCallback)())
{
	DEBUG_INSTANTIATION("Task: iTaskType=" + String(iTaskType) + ",iTicks=" + String(iTicks) + ",iCallback=" + String(iCallback == nullptr ? "nullptr" : "valid"));

	// Initialize task
	mTaskType = iTaskType;
	mTicks = iTicks;
	mTaskCounter = mTicks;
	if (iTaskType == Task::eTaskType::TTriggerOneTime)
	{
		mTaskState = Task::eTaskState::TWaiting;
	}
	else
	{
		mTaskState = Task::eTaskState::TRunning;
	}
	mCallback = iCallback;
}

Task::~Task()
{
	DEBUG_DESTROY("Task");
}

Task *Task::GetNewTask(eTaskType iTaskType, int iTicks, void (*iCallback)(void))
{
	DEBUG_METHOD_CALL("Task::GetNewTask");

	Task *lTask;

	lTask = new Task(iTaskType, iTicks, iCallback);

	// add to task list
	TaskHandler::GetInstance()->GetTaskList()->Add(lTask);

	return lTask;
}

void Task::Process()
{
	DEBUG_METHOD_CALL("Task::Process");

	// Process a single task

	// Check if the task is done or waiting for a trigger
	if ((mTaskState == Task::eTaskState::TDone) || (mTaskState == Task::eTaskState::TWaiting))
	{
		return;
	}

	// Check dependency of tasks
	if ((mTaskType == Task::eTaskType::TFollowUpCyclic) || (mTaskType == Task::eTaskType::TFollowUpOneTime))
	{
		// The previous task must be done
		if (mPreviouslyProcessed != nullptr)
		{
			if (mPreviouslyProcessed->mTaskState != Task::eTaskState::TDone)
			{
				return;
			}
		}
	}

	// Count down
	mTaskCounter -= 1;

	if (mTaskCounter <= 0)
	{
		// Call registered task handler
		// DEBUG_PRINT("Task: "+ this->
		mCallback();

		switch (mTaskType)
		{
		case Task::eTaskType::TCyclic:
		case Task::eTaskType::TFollowUpCyclic:
			// Restart a cyclic task
			mTaskCounter = mTicks;
			break;

		case Task::eTaskType::TTriggerOneTime:
			// Set a startable task to waiting
			mTaskState = Task::eTaskState::TWaiting;
			break;

		default:
			// End this task
			mTaskState = Task::eTaskState::TDone;
			break;
		}
	}
}

void Task::DefinePrevious(Task *iPreviouslyProcessed)
{
	DEBUG_METHOD_CALL("Task::DefinePrevious");

	mPreviouslyProcessed = iPreviouslyProcessed;
}

void Task::Start()
{
	DEBUG_METHOD_CALL("Task::Start");

	if (mTaskState == Task::eTaskState::TWaiting)
	{
		mTaskState = Task::eTaskState::TRunning;
		mTaskCounter = mTicks;
	}
}

void Task::Restart()
{
	DEBUG_METHOD_CALL("Task::Restart");

	if ((mTaskState == Task::eTaskState::TWaiting) || (mTaskState == Task::eTaskState::TRunning))
	{
		mTaskState = Task::eTaskState::TRunning;
		mTaskCounter = mTicks;
	}
}
