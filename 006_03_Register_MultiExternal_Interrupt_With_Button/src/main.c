#include "stm32f4xx.h"

void delay(uint32_t time){while(time--);}
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
	RCC->AHB1ENR=0x00000005; //GPIOA and GPIOC ACTIVED

	GPIOA->MODER=0x00000505; //GPIOA 0-1-4-5 PINS REGISTERED AS OUTPUT
	GPIOA->OTYPER=0x00000000;
	GPIOA->OSPEEDR=0x00000F0F; //PINS REGISTERED AS HIGH SPEED
	GPIOA->PUPDR=0x00000000;

	GPIOC->PUPDR=0x08000002; // GPIOC 13 and 0th pins set as pulldown
	                          //FURTHER REGISTERS WILL BE SUFFICIENT WHEN IT IS DEFAULT



}
void EXTI_Config(void)
{
	RCC->APB2ENR=0x00004000; //SystemCFG active for EXTI

	SYSCFG->EXTICR[0]=0x00000002;
	SYSCFG->EXTICR[2]=0x00000200;
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_SetPriority(EXTI15_10_IRQn,1);


	EXTI->IMR=0x00002001; // PC 0 and PC 13 MASKED AS INTERRUPT
	EXTI->RTSR=0x00002001; // RISING TRIGGER ENABLED


}
void EXTI15_10IRQHandler()
{
	if(EXTI->PR & 0x00002000)
	{
		GPIOA->ODR=0x00000030;
		delay(33600000);

		EXTI->PR = 0x00002000;

	}
}
void EXTI0_IRQHandler()
{
 if(EXTI->PR & 0x00000001)
 {
	 GPIOA->ODR=0x00000003; // FIRST TWO LEDS WILL BE TURNED ON
	 delay(33600000);

	 EXTI->PR = 0x00000001; // FLAG IS CLEARED

 }


}

int main(void)
{
RCC_Config();
GPIO_Config();
EXTI_Config();

	while (1)
  {

   GPIOA->ODR=0x00000033; //OUTPUT PINS SET TO 1


  }
}
