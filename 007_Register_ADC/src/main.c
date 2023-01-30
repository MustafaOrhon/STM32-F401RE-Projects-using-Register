#include "stm32f4xx.h"
uint8_t adc_value;
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
  RCC->AHB1ENR |= 0x00000001; // GPIOA ENABLED;

  GPIOA->MODER |= 0x00000003; //Pin 0 Analog
  GPIOA->OSPEEDR |=0x00000003;  //Pin 0 100 MHz



}
void ADC_Config(void)
{
	RCC->APB2ENR|= 0x00000100; // ABP2 Clock Enabled

	ADC1->CR1 |= 0x02000000; //Resolution set to 8-bit;
	ADC1->CR2 |= 0x00000001; //ADC ENABLED

	ADC1->SMPR2 |= 0x00000003; //56 Cycle Selected for channel 0;
	ADC->CCR |= 0x00010000;




}
uint8_t Read_ADC()
{
	uint8_t value;
	ADC1->CR2 |= 0x40000000; //SOFTWARE CONVERSETION START

	while(!(ADC1->SR & 0x00000002));
	value= ADC1->DR;

	return value;

}

int main(void)
{
RCC_Config();
GPIO_Config();
ADC_Config();
  while (1)
  {

adc_value=Read_ADC();


  }
}
