#include "OS.h"

void (*TaskList[MAX_TASK])(void)={
		//			Task function	Task id		Task prio
		Task1,
		Task2,
		Task3,

};

void SetTaskPending (_E_Task_Name TaskNr)
{
    if(TaskNr < MAX_TASK)
    TaskParams_u8[TaskNr-1][TASK_PENDING]=1;
}


uint8_t TaskParams_u8[MAX_TASK][MAX_TASK_PARAMS]={
		           //Prio,		Periodic,	Period,		TicksToRun,	Pending, 	Running
		/*1*/  		{0,			0,  		250,		0,			0,			0}, //Process the data in AFRxBuffer
		/*2*/		{0,			0,			250,		0,			0,			0}, //Compact and save the data in flash (has to be executed after the task that process the data from AFRxBuffer
		/*3*/		{0,			0,			255,	   	0,			0,			0}, //Watchdog Reload

};

//=================================The schedular for the 17 task's==========================//
void os_start( void )
{
	uint8_t     TaskCounter, PrioCounter;
	while (1)
	{
		getRstSource();
		//==================================Update Task list==============================//
		for(TaskCounter=0; TaskCounter<MAX_TASK; TaskCounter++)
		{
			if(TaskParams_u8[TaskCounter][TASK_PERIODIC])
			{
				if(TaskParams_u8[TaskCounter][TASK_PERIOD] && TaskParams_u8[TaskCounter][TASK_TTR])
				{
					--TaskParams_u8[TaskCounter][TASK_TTR];
				}
				else
				{
					TaskParams_u8[TaskCounter][TASK_TTR] = TaskParams_u8[TaskCounter][TASK_PERIOD];
					TaskParams_u8[TaskCounter][TASK_PENDING] = 1;
				}
			}
		}
		//=========================================================================//

		//====================================Tasker=================================//
		PrioCounter = 0;
		TaskCounter = 0;
		for(PrioCounter=0; PrioCounter<MAX_TASK_PRIO; PrioCounter++)
		{
			for(TaskCounter=0; TaskCounter<MAX_TASK; TaskCounter++)
			{
				if(TaskParams_u8[TaskCounter][TASK_PRIO]==PrioCounter && TaskParams_u8[TaskCounter][TASK_PENDING])
				{
					TaskParams_u8[TaskCounter][TASK_PENDING]=0;
					TaskParams_u8[TaskCounter][TASK_RUNNING]=1;
					(*TaskList[TaskCounter])();
					TaskParams_u8[TaskCounter][TASK_RUNNING]=0;
				}
			}
		}

	}
}
