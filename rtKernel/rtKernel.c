/*******************************************************************************
* Title                 :   real time kernel module
* Filename              :   tKernel.c
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f4xx.h"
#include "rtKernel.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define BUS_FREQ         16000000
#define NUM_OF_THREADS   3
#define STACK_SIZE       100
#define SYSPRI3                 (*((volatile uint32_t *)0xE000ED20))
#define INTCTRL_SYSTICK         (*((volatile uint32_t *)0xE000ED04))

#define TASK_3_PERIOD    500


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
struct __tcb
{
  uint32_t *stackPtr;
  struct __tcb *nextPtr;
};

typedef struct __tcb tcbType;
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint32_t MILIS_PRESCALER;
tcbType *tcb_currentPtr;
tcbType tcbs[NUM_OF_THREADS];
uint32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];
volatile uint32_t period_tick;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void rtSchedulerLaunch(void);
void rtKernelStackInit(int32_t task_index);
void GPIO_Init(void);
void Toggle_LED(void);
/******************************************************************************
* Function Definitions
*******************************************************************************/

/******************************************************************************
* Function : rtKernelStackInit()
*//**
* @brief
* @param  int32_t task_index
* @retval none
* @note
*******************************************************************************/
void rtKernelStackInit(int32_t task_index)
{
  tcbs[task_index].stackPtr = &TCB_STACK[task_index][STACK_SIZE-16];
  TCB_STACK[task_index][STACK_SIZE - 1]   = (1 << 24);               // xPSR should be 1 to tell the CPU it executing in thumb mode
}
/*************** END OF FUNCTIONS ***************************************************************************/


/******************************************************************************
* Function : rtKernelAddThread()
*//**
* @brief
* @param
* @retval none
* @note
*******************************************************************************/
uint8_t rtKernelAddThread(void(* task_0)(void),
                          void(* task_1)(void),
                          void(* task_2)(void))
{
  __disable_irq();

  tcbs[0].nextPtr = &tcbs[1];     // threaded added in circular linked list
  tcbs[1].nextPtr = &tcbs[2];
  tcbs[2].nextPtr = &tcbs[0];

  rtKernelStackInit(0);
  TCB_STACK[0][STACK_SIZE - 2] = (int32_t)task_0;
  rtKernelStackInit(1);
  TCB_STACK[1][STACK_SIZE - 2] = (int32_t)task_1;
  rtKernelStackInit(2);
  TCB_STACK[2][STACK_SIZE - 2] = (int32_t)task_2;

  tcb_currentPtr = &tcbs[0];

  __enable_irq();

  return 1;
}
/*************** END OF FUNCTIONS ***************************************************************************/

/******************************************************************************
* Function :rtKernelInit()
*//**
* @brief
* @param  none
* @retval none
* @note
*******************************************************************************/
void rtKernelInit(void)
{
  __disable_irq();
  MILIS_PRESCALER = (BUS_FREQ/1000);
}
/*************** END OF FUNCTIONS ***************************************************************************/

/******************************************************************************
* Function : rtKernelLaunch()
*//**
* @brief  In this routine initialize systeic timer equal to quanta size and
*         launch first task
* @param  uint32_t quanta
* @retval none
* @note
*******************************************************************************/
void rtKernelLaunch(uint32_t quanta)
{
  //
  SysTick->LOAD  = (quanta* MILIS_PRESCALER)-1;
  SYSPRI3        = (SYSPRI3  & 0x00FFFFFF)|0xE0000000;                     // set Priority for Systick Interrupt to 7
  SysTick->VAL   = 0UL;                                                    // Load the SysTick Counter Value
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;
  rtSchedulerLaunch();
}
/*************** END OF FUNCTIONS ***************************************************************************/

/******************************************************************************
* Function : rtThreadYield()
*//**
* @brief  generate software systic interrupt
* @param  none
* @retval none
* @note
*******************************************************************************/
void rtThreadYield()
{
  INTCTRL_SYSTICK |= 0x04000000; // triger systick
}
/*************** END OF FUNCTIONS ***************************************************************************/



/******************************************************************************
* Function : rtSchedulerRoundRobin(void)
*//**
* @brief  this function schedule periodic task and context switching
* @param  none
* @retval none
* @note
*******************************************************************************/
void rtSchedulerRoundRobin(void)
{
  period_tick++;
  if((++period_tick) == TASK_3_PERIOD)
  {
    (*Task3)();
    period_tick=0;
  }
  tcb_currentPtr =  tcb_currentPtr->nextPtr;
}
/*************** END OF FUNCTIONS ***************************************************************************/

