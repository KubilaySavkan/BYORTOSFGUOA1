#include "osKernel.h"

#define SYSPRI3						(*((volatile uint32_t *)0xE000ED20))		// !TODO: find another way to access
	
void osSchedulerLaunch(void);

#define NUM_OF_THREADS		3u
#define STACKSIZE					100u

#define BUS_FREQ					16000000		// !TODO: correct this value
uint32_t	ui32_MS_PRESCALER;

struct tcb
{
	int32_t			*stackPt;
	struct tcb	*nextPt;
};
typedef struct tcb tcb_t;

tcb_t			tcbs[NUM_OF_THREADS];
tcb_t *		currentPt;
int32_t		TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(uint8_t i)
{
	tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE-16];	// sp of each thread points to the top of stack
	TCB_STACK[i][STACKSIZE-1] = 0x01000000;					// init xPSR, set bit24 to indicate that processor will run in thumb mode
}

uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void))
{
	__disable_irq();
	
	// loop: task0 -> task1 -> task2
	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[0];
	
	// init stacks and pcs
	osKernelStackInit(0);
	TCB_STACK[0][STACKSIZE-2] = (int32_t)(task0);		// pc
	osKernelStackInit(1);
	TCB_STACK[1][STACKSIZE-2] = (int32_t)(task1);
	osKernelStackInit(2);
	TCB_STACK[2][STACKSIZE-2] = (int32_t)(task2);
	
	// begin with first thread
	currentPt = &tcbs[0];
	
	__enable_irq();
	
	return 1;
}

void osKernelInit(void)
{
	__disable_irq();
	
	ui32_MS_PRESCALER = (BUS_FREQ / 1000u);
	
	//__enable_irq();
}

void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL = 0u;
	SysTick->VAL  = 0u;
	SYSPRI3       = ((SYSPRI3 & 0x00FFFFFF) | 0xE0000000);		// Priority 7 - lowest
	SysTick->LOAD = ((quanta * ui32_MS_PRESCALER) - 1);
	SysTick->CTRL = 0x00000007;
	
	osSchedulerLaunch();
}

