#include "uart.h"
#include  "stm32l0xx.h"

#define SYS_FREQ		2097000
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE	9600


static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk  +  (BaudRate/2U))/BaudRate);
}



void uart_init()
	{
	/*Enable clock access to GPIOC*/
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER|=GPIO_MODER_MODE2_1;
	GPIOA->MODER&=~GPIO_MODER_MODE2_0;
	#define AF04 0x04
	GPIOA->AFR[0]|=(AF04<<8);

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	USART2->BRR=compute_uart_bd(APB1_CLK,UART_BAUDRATE);

	/*Configure transfer direction*/
    USART2->CR1 = USART_CR1_TE;

	/*Enable uart module*/
    USART2->CR1 |= USART_CR1_UE;

	}





void uart2_write(int ch)
{
	/*Make sure transmit data register is empty*/
	while(!(USART2->ISR & USART_ISR_TXE)){}

	/*Write to the transmit data register*/
	USART2->TDR =  (ch & 0xFF);
}

int __io_putchar(int ch)
	{
	uart2_write(ch);
	return ch;
	}
