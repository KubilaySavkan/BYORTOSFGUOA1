#include <stdint.h>

#include "osKernel.h"

#define QUANTA		10u

volatile uint32_t ui32TaskCnt0, ui32TaskCnt1, ui32TaskCnt2;

void Task0(void)
{
	while(1)
	{
		ui32TaskCnt0++;
		osThreadYield();
	}
}

void Task1(void)
{
	while(1)
	{
		ui32TaskCnt1++;
		osThreadYield();
	}
}

void Task2(void)
{
	while(1)
	{
		ui32TaskCnt2++;
		//osThreadYield();
	}
}

int main(void)
{
	osKernelInit();
	osKernelAddThreads(&Task0, &Task1, &Task2);
	osKernelLaunch(QUANTA);
}

