
#include "stm32f4xx.h"
uint32_t data;
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
void FLASH_Unlocker()
{
	while((FLASH->SR & 0x00010000) != 0); // WAIT UNTIL BUSY BIT IS 1

  	FLASH->KEYR =  0x45670123;
  	FLASH->KEYR =  0xCDEF89AB;  // FLASH KEY OPENED FOR WRITING
}
void FLASH_Erase()  // SECTOR 7
{
	while((FLASH->SR & 0x00010000) != 0); // WAIT UNTIL BUSY BIT IS 1
	FLASH->CR |= 1<<1; // SET SECTOR ERASE
	FLASH->CR |= 0x00000038;; // SET START BIT
	FLASH->CR |= 1<<16; // SET START BIT

	while((FLASH->SR & 0x00010000) != 0); // WAIT UNTIL BUSY BIT IS 1

	if((FLASH->SR & 0x00000001) != 0)
	{

		FLASH->SR |= 0x00000001;

	}
	else
	{
		FLASH_Erase();
	}
	FLASH->CR &= ~(1<<1);

}
void FLASH_Write(uint32_t address,uint32_t data)
{
	while((FLASH->SR & 0x00010000) != 0); // WAIT UNTIL BUSY BIT IS 1
	FLASH->CR |= 1<<0;  // SET PG BIT TO 1
    FLASH->CR |= 2<<8; // SET PARALELL TO 32

    *(__IO uint32_t*)address = data;

    while((FLASH->SR & 0x00010000) != 0); // WAIT UNTIL BUSY BIT IS 1
}

uint32_t FLASH_Read(uint32_t address)
{
   	uint32_t myFlashdata;
   	myFlashdata = *(uint32_t *)address;

   return myFlashdata;
}
void FLASH_Locker()
{
	while((FLASH->SR & 0x00010000) != 0); // WAIT UNTIL BUSY BIT IS 1
	FLASH->CR |= 1<<31; // SET LOCK BIT TO 1

}


int main(void)
{

 RCC_Config();
 FLASH_Unlocker();
 FLASH_Erase();
 FLASH_Write(0x08060000,0xCC);
 FLASH_Locker();
 FLASH_Unlocker();
 data = FLASH_Read(0x08060000);
 FLASH_Locker();
  while (1)
  {

  }
}
