#include "generic.h"
typedef enum
{
    Task_0 = 0,
    Task_1,
    Task_2,
    Task_3,

}_E_Task_Name;

#define countof(a)   (sizeof(a) / sizeof(*(a)))
/* Uncomment the line below if you will use the SPI peripheral as a Master */


#define MAX_TASK		                                            17
#define MAX_TASK_PARAMS	                                            6
#define MAX_TASK_PRIO	                                            2//1		//0 is highest

#define TASK_PRIO		                                            0
#define TASK_PERIODIC	                                            1
#define TASK_PERIOD		                                            2
#define TASK_TTR		                                            3		//TicksToRun
#define TASK_PENDING	                                            4
#define TASK_RUNNING	                                            5

#define TICK	                                                    1

//-------------------Variable definitions------------------------------------------------------------//
static __IO uint32_t TimingDelay;
u8 TaskParams_u8[MAX_TASK][MAX_TASK_PARAMS];
