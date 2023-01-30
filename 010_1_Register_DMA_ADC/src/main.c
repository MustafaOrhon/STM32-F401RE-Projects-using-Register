
#include "stm32f4xx.h"
uint16_t adc_value[1];
void RCC_Config(void)
{
 RCC->CR &= ~(1<<0); //  HSI OFF
 RCC->CR |= 1 << 16;  // HSE ON
 while(!(RCC->CR & (1<<17)) ); //Wait the flag
 RCC->CR |= 1 << 19; // CSSON ON for Security
 RCC->CR |= 1<<24;
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

   ADC1->CR1 |= 1<<8; // Scan Mode Enabled Incase We want to Use Multiple ADC
   ADC1->CR1 |= 0<<24;

   ADC1->CR2 |= 1<<0;  //ADC Enabled
   ADC1->CR2 |= 1<<1; //Continious Mode Enabled
   ADC1->CR2 |= 1<<8; //DMA Enabled
   ADC1->CR2 |= 1<<9;  //DDS Enabled for Continous Conv.
   ADC1->CR2 |= 1<<10; //EOC Enabled
   ADC1->CR2 |= 1<<30; //Start Conversation

   ADC1->SQR1 |= 0<<20;
   ADC1->SQR3 |= 0<<0;


}
void DMA_Config(void)
{
	RCC->AHB1ENR |= 1<<22; //DMA2 Clock Enabled

	while(DMA2_Stream4->CR & 0x00000001); //Wait stream4 to be 0
	DMA2_Stream4->PAR |= (uint32_t) &ADC1->DR;
	DMA2_Stream4->M0AR |= (uint32_t)&adc_value;
	DMA2_Stream4->CR |= 0<<6;  // Data Transfer To Periph TO MEMORY
	DMA2_Stream4->CR |= 1<<8;  // Circiular Mode On
	DMA2_Stream4->CR |= 1<<10;  //Memory Increment ON
	DMA2_Stream4->CR |= 1<<11; //Periph Data Size Half Word
	DMA2_Stream4->CR |= 1<<13; // Memory Data Size Half Word

	DMA2_Stream4->CR |= 1<<16; //Priority Level Very High
	DMA2_Stream4->CR |= 1<<17;

	DMA2_Stream4->CR |= 0<<25;
	DMA2_Stream4->NDTR |=1; //Single Channel


	DMA2_Stream4->CR |= 1<<0;

}


int main(void)
{
RCC_Config();
GPIO_Config();
ADC_Config();
DMA_Config();

ADC1->CR2 |= ADC_CR2_SWSTART;
  while (1)
  {

  }
}
