#ifndef _OSKERNEL_H
#define _OSKERNEL_H

#include "stm32f4xx.h"

#include <stdint.h>

uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void));

void osKernelInit(void);

void osKernelLaunch(uint32_t quanta);

void osThreadYield(void);

#endif//_OSKERNEL_H

