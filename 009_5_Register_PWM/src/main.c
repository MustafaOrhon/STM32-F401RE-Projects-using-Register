
#include "stm32f4xx.h"
uint16_t count = 0;
void RCC_Config(void)
{
 RCC->CR &= ~(1<<0); //  HSI OFF
 RCC->CR |= 1 << 16;  // HSE ON
 while(!(RCC->CR & (1<<17)) ); //Wait the flag
 RCC->CR |= 1 << 19; // CSSON ON for Security
 RCC->PLLCFGR =0x00000000;

 RCC->PLLCFGR |= (1<<22) ; //PLL Source HSE

 RCC->PLLCFGR |= (25 << 0) ; //PLL M 25
 RCC->PLLCFGR |=  (168 << 6) ;   // PLL N 168
 RCC->PLLCFGR &= ~(1<<16); // PLL P 2
 RCC->PLLCFGR &= ~(1<<17);

 RCC->CR |=(1<<24); //PLL ON

 while(!(RCC->CR & (1<<25)) ); //Wait PLL Active

 RCC->CFGR &= ~(1<<0);
 RCC->CFGR |= (1<<1);  //System Clock PLL

 while(!(RCC->CFGR & (1<<1)) );
 RCC->CFGR &= ~(1<<2);
 RCC->CFGR |= (1<<3);  //System Clock PLL
}

void GPIO_Config()
{
	RCC->AHB1ENR |= 1<<0 ; //AHB1 GPIOA ENABLED

	GPIOA->MODER = 0x0000000A; //Pin 0 and Pin 1 Defined as Alterna Functions
    GPIOA->AFR[0] = 0x00000011; // Pin0 and Pin1 Defined as Timer-2



}
void TIMER_Config()
{

RCC->APB1ENR |= 0x00000001; // TIMER-2 ENABLED


TIM2->CR1 &= ~(1<<4); //Counter used as upcounter
TIM2->CR1 &= ~(1<<5); //Edge-Aligned
TIM2->CR1 &= ~(1<<6);
TIM2->CR1 &= ~(1<<8); //Set Clock Division to 1;
TIM2->CR1 &= ~(1<<9);

TIM2->SMCR &=~(1<<0);
TIM2->SMCR &=~(1<<1);
TIM2->SMCR &=~(1<<2);

TIM2->CCMR1 = 0x0006060; // Capture Compare 1&2 OUTPUT AND PWM SELECTED
TIM2->CCER = 0x00000011; //CC1-CC2 Channel Output Enabled

TIM2->PSC = 8399;
TIM2->ARR = 9999;
TIM2->CCR1 = 9999;
TIM2->CCR2 = 2499;

TIM2->CR1 |= 1<<0; // Counter Enable
}
int main(void)
{
RCC_Config();
GPIO_Config();
TIMER_Config();
  while (1)
  {

  }
}
