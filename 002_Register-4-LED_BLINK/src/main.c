
#include "stm32f4xx.h"
void delay (uint32_t  time)
{
	while(time--);
}
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

void GPIO_Config(void)
{
	RCC->AHB1ENR |= (1<<0); // AHB1 A Port Enable


	GPIOA->MODER  |= 0x00000505; // GPIOA 0,1,4,5 Pins registered as output
	GPIOA->OSPEEDR |= 0x00000A0A; // GPIOA 0,1,4,5 Pins registered as high speed


}
int main(void)
{
  RCC_Config();
  SystemCoreClockUpdate();

  GPIO_Config();

  while (1)
  {
         GPIOA->ODR |= 1<<0; // 0. PIN SET
         GPIOA->ODR |= 1<<1; // 1. PIN SET
		 GPIOA->ODR |= 1<<4; // 4. PIN SET
		 GPIOA->ODR |= 1<<5; // 5. PIN SET
		 delay(16800000);
         GPIOA->ODR &= ~(1<<0); // 0. PIN RESET
         GPIOA->ODR &= ~(1<<1); // 1. PIN RESET
         GPIOA->ODR &= ~(1<<4); // 4. PIN RESET
         GPIOA->ODR &= ~(1<<5) ;// 5. PIN RESET
         delay(16800000);
  }
}
