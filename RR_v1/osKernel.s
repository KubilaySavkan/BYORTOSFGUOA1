			AREA |.text|, CODE, READONLY, ALIGN=2
			THUMB
			EXTERN currentPt
			EXPORT SysTick_Handler
			EXPORT osSchedulerLaunch

SysTick_Handler							; SAVE R0, R1, R2, R3, R12, LR, PC, PSR
	CPSID		I						; DISABLE IRQ
	PUSH		{R4-R11}				; SAVE R4, R5, R6, R7, R8, R9, R10, R11
	LDR			R0, =currentPt			; R0 POINTS TO currentPt
	LDR			R1, [R0]				; R1 = currentPt
	STR			SP, [R1]				; SP = currentPt->stackPt
	LDR			R1, [R1, #4]			; R1 = currentPt->nextPt
	STR			R1, [R0]				; currentPt = R1
	LDR			SP, [R1]				; SP = currentPt->stackPt
	POP			{R4-R11}
	CPSIE		I						; ENABLE IRQ
	BX			LR

osSchedulerLaunch
	LDR			R0, =currentPt			; R0 POINTS TO currentPt
	LDR			R2, [R0]				; R2 =currentPt
	LDR			SP, [R2]				; SP =currentPt->stackPt
	POP			{R4-R11}
	POP			{R0-R3}
	POP			{R12}
	ADD			SP, SP, #4
	POP			{LR}
	ADD			SP, SP, #4
	CPSIE		I
	BX			LR

	ALIGN
	END

