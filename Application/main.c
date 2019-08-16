/*
 * main.c
 *
 *  Created on: 04-Aug-2019
 *      Author: S.K.DUBEY
 */

#include "stm32f4xx.h"

#define RED         (1U<<5) //D13-PA5
#define GREEN       (1U<<6) //D12-PA6
#define BLUE        (1U<<7) //D11-PA7
#define ORANGE      (1U<<8) //D10-PA8

#define RED_BIT     (1U<<10)
#define GREEN_BIT   (1U<<12)
#define ORANGE_BIT  (1U<<14)
#define BLUE_BIT    (1U<<16)

#define GPIOA_CLOCK (1<<0)

void GPIO_Init(void);
uint32_t getTick(void);
void Delay_sec(uint32_t seconds);

void blueOff(void);
void blueOn(void);

void greenOff(void);
void greenOn(void);

void redOff(void);
void redOn(void);

void orangOff(void);
void orangeOn(void);

int red_main(void);
int green_main(void);

volatile uint32_t tick;
volatile uint32_t _tick;

int main()
{
  uint32_t volatile start = 0;
  GPIO_Init();

  if(start)
  {
    green_main();
  }
  else
  {
    red_main();
  }
  while(1)
  {
  }
          /*this inifinite loop I brought these
          unctions here I had to keep the while
          one in the original main function in order to
          make the decode not terminate.But when we put it
          here it remains and it uses time to switch in between the blue main and then the
          orange moon*/
}

void GPIO_Init(void)
{
  RCC->AHB1ENR |= GPIOA_CLOCK;
  GPIOA->MODER |= RED_BIT|GREEN_BIT|ORANGE_BIT|BLUE_BIT;
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100U);
  __enable_irq();
}

void SysTick_Handler(void)
{
  ++tick;
}

uint32_t getTick(void)
{
  __disable_irq();
  _tick = tick;
  __enable_irq();
  return _tick;
}


void Delay_sec(uint32_t seconds)
{
  uint32_t temp =getTick();
  seconds+=100;
  while((getTick()-temp)<seconds){}
}

void blueOn(void)
{
  GPIOA->ODR |= BLUE;
}

void blueOff(void)
{
  GPIOA->ODR &= ~BLUE;
}

void orangeOn(void)
{
 GPIOA->ODR |= ORANGE;
}

void orangeOff(void)
{
  GPIOA->ODR &= ~ORANGE;
}


void redOn(void)
{
  GPIOA->ODR |= RED;
}

void redOff(void)
{
  GPIOA->ODR &= ~RED;
}

void greenOn(void)
{
  GPIOA->ODR |= GREEN;
}

void greenOff(void)
{
  GPIOA->ODR &= ~GREEN;
}




int red_main()
{
  while(1)
  {
    redOn();
    Delay_sec(1);
    redOff();
    Delay_sec(1);
  }
}

int green_main()
{
  while(1)
  {
    greenOn();
    Delay_sec(1);
    greenOff();
    Delay_sec(1);
  }
}
