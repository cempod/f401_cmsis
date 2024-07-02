#include "main.h"
#include <stdint.h>
#include "stm32f4xx.h"

volatile uint32_t delayTimerValue = 0;

//	SysTick interrupt handle
void SysTick_Handler(void)
{
	delayTimerValue--;
}


//	RCC initialization
static void 
init_clock(void)
{
	//	set flash latency to one wait state
	FLASH->ACR |= FLASH_ACR_LATENCY;

	RCC->CR |= RCC_CR_HSEON;

	//	check HSI48 clock source ready flag
	while(((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY));

    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN,
             RCC_PLLCFGR_PLLSRC_HSE | (RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_0) | 168 << RCC_PLLCFGR_PLLN_Pos);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, 0x00000000U);

    SET_BIT(RCC->CR, RCC_CR_PLLON);

    while(((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY));

    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    while(((uint32_t)(READ_BIT(RCC->CFGR, RCC_CFGR_SWS)))!=RCC_CFGR_SWS_PLL);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);

    SysTick_Config(84000000 / 1000);

    SystemCoreClock = 84000000;
}

static void 
init_gpio(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    GPIOC->MODER &= ~GPIO_MODER_MODER13;
	GPIOC->MODER |= GPIO_MODER_MODER13_0;

    GPIOC->OTYPER &= ~GPIO_OTYPER_OT_13;

    GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED13;

    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

    GPIOC->BSRR |= GPIO_BSRR_BR_13;
}

//	set delay in miliseconds using sysTick timer
static void delay(uint32_t delay)
{
	delayTimerValue = delay;

	while(delayTimerValue);
}

int main(void){
    init_clock();

    init_gpio();
    while (1)
    {
        GPIOC->BSRR |= GPIO_BSRR_BS_13;
		delay(1000);

		GPIOC->BSRR |= GPIO_BSRR_BR_13;
		delay(1000);
    }
    
    return 0;
}