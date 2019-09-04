/*******************************************************************************
* Title                 :   main module
* Filename              :   main.c
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f4xx.h"
#include "rtKernel.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define QUANTA  10

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

uint32_t count0,count1,count2;

/******************************************************************************
* Function Definitions
*******************************************************************************/

//task 0 just increment counter
void Task0(void)
{
  while(1)
  {
    count0++;
  }

}

//task 1 just increment counter
void Task1(void)
{
  while(1)
  {
    count1++;
    rtThreadYield();
  }

}

//task 2 just increment counter
void Task2(void)
{
  while(1)
  {
    count2++;
    rtThreadYield();
  }

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
  rtKernelInit();
  rtKernelAddThread(&Task0,&Task1,&Task2);
  rtKernelLaunch(QUANTA);
}
/*************** END OF FUNCTIONS ***************************************************************************/

