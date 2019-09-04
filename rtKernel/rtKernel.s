;*****************************************************************************
;* File Name          : rtKernel.s
;* Author             : SKD
;*******************************************************************************

    AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
    EXTERN  tcb_currentPtr
    EXPORT  SysTick_Handler
    EXPORT  rtSchedulerLaunch

SysTick_Handler             ;save r0,r1,r2,r3,r12,lr,pc,psr
    CPSID   I
    PUSH    {R4-R11}             ;save r4,r5,r6,r7,r8,r9,r10,r11
    LDR     R0, =tcb_currentPtr  ; r0 points to tcb_currentPtr
    LDR     R1, [R0]        ; r1= tcb_currentPtr
    STR     SP, [R1]        ; update stack pointer of current tcb block
    LDR     R1, [R1,#4]     ; r1 = tcb_currentPtr->next
    STR     R1, [R0]        ; update next thread tcb_currentPtr = r1
    LDR     SP, [R1]        ; Load next thread SP = tcb_currentPtr->stackPt
    POP     {R4-R11}
    CPSIE   I
    BX      LR


; pop value from task0 stack int register annd set stack pinter to current tread stack
rtSchedulerLaunch
    LDR     R0, =tcb_currentPtr
    LDR     R2, [R0]       ; R2 =currentPt
    LDR     SP, [R2]       ; SP = tcb_currentPtr->stackPt
    POP     {R4-R11}
    POP     {R0-R3}
    POP     {R12}
    ADD     SP,SP,#4       ; load PC value to lr to jum to tsk0
    POP     {LR}           ; jump from here to task0
    ADD     SP,SP,#4
    CPSIE   I
    BX      LR

    ALIGN
    END