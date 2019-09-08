/*******************************************************************************
* Title                 :   main module
* Filename              :   main.c
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f4xx.h"
#include "rtKernel.h"
#include "uart_cli.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define QUANTA  1
#define RED         (1U<<5) //D13-PA5
#define RED_BIT     (1U<<10)
#define GPIOA_CLOCK (1<<0)
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint32_t count0,count1,count2;

int32_t semaphore1, semaphore2;
/******************************************************************************
* Function Prototypes
*******************************************************************************/
void Toggle_LED(void);
void GPIO_Init(void);
/******************************************************************************
* Function Definitions
*******************************************************************************/

//task 0
void Task0(void)
{
  while(1)
  {
    count0++;
  }

}

//task 1, using two semaphore trying to run both task 1 aand task2 at the same time
void Task1(void)
{
  while(1)
  {
    rtSignalSet(&semaphore1);
    rtSignalWait(&semaphore2);
    count1++;
  }

}

//task 2 just increment counter
void Task2(void)
{
  while(1)
  {
    rtSignalSet(&semaphore2);
    rtSignalWait(&semaphore1);
    count2++;
  }

}

// periodic scheduled task 100ms
void Task3(void)
{
  Toggle_LED();
}

// periodic schedule task 250ms
void Task4(void)
{
  //Probe_CH3();
}


/******************************************************************************
* Function : main(void)
*//**
* @brief  after startup code program comes here (called from reset handler)
* @param  none
* @retval int
* @note
*******************************************************************************/
int main(void)
{
  GPIO_Init();
  UART_CLIInit();

  rtSemaphoreInit((int32_t *)&semaphore1,1);
  rtSemaphoreInit((int32_t *)&semaphore2,0);
  rtKernelInit();
  rtKernelAddThread(&Task0,&Task1,&Task2);
  rtKernelLaunch(QUANTA);
}
/*************** END OF FUNCTIONS ***************************************************************************/
/******************************************************************************
* Function : GPIO_Init(void)
*//**
* @brief  Initialize RED LED which is on Development kit
* @param  none
* @retval none
* @note
*******************************************************************************/
void GPIO_Init(void)
{
  RCC->AHB1ENR |= GPIOA_CLOCK;
  GPIOA->MODER |= RED_BIT;
}
/*************** END OF FUNCTIONS ***************************************************************************/
/******************************************************************************
* Function : Toggle_LED(void)
*//**
* @brief
* @param  none
* @retval none
* @note
*******************************************************************************/
void Toggle_LED(void)
{
  GPIOA->ODR ^= RED;
}
/*************** END OF FUNCTIONS ***************************************************************************/
