
#include "stm32f4xx.h"
uint16_t adc_value[1];
char Rx_Buff[100];
int i=0;
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

  GPIOA->MODER |= 0x000000A0; //Pin 2*3 Analog
  GPIOA->AFR[0] |= 0x00007700; // Pin 2,3 Selected as AF7 (USART2)



}
void USART_Config(void)
{

	RCC->APB1ENR |= 1<<17; // USART2 Clock Enabled

	USART2->BRR = 0x1112; // Baud Rate Set To 9600
    USART2->CR1 = 1<<2 ; //Receiver Enabled
    USART2->CR1 = 1<<3 ; //Transmit Enabled
    USART2->CR1 = 1<<5 ; //Rxne interrupt enabled


    USART2->CR1 = 1<<13 ; //USART Enabled
}

void NVIC_Config()
{
  NVIC->ISER[1] |= 1<<7;

}
void USART2_IRQHandler()
{
   volatile int Str;
   Str = USART2->SR;
   Rx_Buff[i] = USART2->DR;
   i++;
}
void Send_Char(char message)
{
	while(!(USART2->SR & 0x00000080));  //Wait until tx buffer empty
	USART2->DR = message;


}
void Send_Message(char *str)
{
   while(*str)
   {

	   Send_Char(*str);
	   str++;
   }
}

int main(void)
{
RCC_Config();
GPIO_Config();
USART_Config();
NVIC_Config();

  while (1)
  {
    Send_Message("Hello world");
    for(int i=0;i<10000000;i++);


  }
}
