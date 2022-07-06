/*
 * i2c.c
 *
 *  Created on: Jul 5, 2022
 *      Author: hussamaldean
 */

#include "i2c.h"

const uint8_t AF04 =0x04;

void i2c_init(uint32_t timing)
{
	RCC->IOPENR|=RCC_IOPENR_GPIOBEN;
	GPIOB->MODER&=~(GPIO_MODER_MODE8|GPIO_MODER_MODE9);
	GPIOB->MODER|=GPIO_MODER_MODE8_1|GPIO_MODER_MODE9_1;
	GPIOB->OTYPER|=GPIO_OTYPER_OT_8|GPIO_OTYPER_OT_9;
	GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEED8_0|GPIO_OSPEEDER_OSPEED8_1|GPIO_OSPEEDER_OSPEED9_0|GPIO_OSPEEDER_OSPEED9_1;
	GPIOB->AFR[1]|=(AF04<<0)|(AF04<<4);

	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;
	I2C1->CR1 &=~I2C_CR1_PE;
	I2C1->TIMINGR=timing;
	I2C1->CR1 |=I2C_CR1_PE;



}

void i2_read(uint8_t slav_add, uint8_t memadd, uint8_t *data, uint8_t length )
	{
	(slav_add=slav_add<<1);
	/*Enable I2C*/
	I2C1->CR1 |=I2C_CR1_PE;
	/*Set slave address*/
	I2C1->CR2=(slav_add<<I2C_CR2_SADD_Pos);
	/*7-bit addressing*/
	I2C1->CR2&=~I2C_CR2_ADD10;
	/*Set number to transfer to 1 for write operation*/
	I2C1->CR2|=(1<<I2C_CR2_NBYTES_Pos);
	/*Set the mode to write mode*/
	I2C1->CR2&=~I2C_CR2_RD_WRN;
	/*Software end*/
	I2C1->CR2&=~I2C_CR2_AUTOEND;
	/*Generate start*/
	I2C1->CR2|=I2C_CR2_START;
	/*Wait until transfer is completed*/
	while(!(I2C1->ISR & I2C_ISR_TC)){
		/*Check if TX buffer is empty*/
		if(I2C1->ISR & I2C_ISR_TXE)
		{
			/*send memory address*/
			I2C1->TXDR = (memadd);
		}
	}
	/*Reset I2C*/
	I2C1->CR1 &=~I2C_CR1_PE;
	I2C1->CR1 |=I2C_CR1_PE;
	/*Set slave address*/
	I2C1->CR2=(slav_add<<I2C_CR2_SADD_Pos);
	/*Set mode to read operation*/
	I2C1->CR2|=I2C_CR2_RD_WRN;
	/*Set length to the required length*/
	I2C1->CR2|=((length)<<I2C_CR2_NBYTES_Pos);
	/*aut generate stop after transfer completed*/
	I2C1->CR2|=I2C_CR2_AUTOEND;
	/*Generate start*/
	I2C1->CR2|=I2C_CR2_START;
	/*wait until stop is generated*/
	while(!(I2C1->ISR & I2C_ISR_STOPF))
			{
				/*If RX buffer is empty*/
			  if(I2C1->ISR &(I2C_ISR_RXNE))
			  	  {
				  /*read the data and increment the pointer*/
				  *data++=I2C1->RXDR;
				 }
			}
	/*disable the peripheral*/
	I2C1->CR1 &=~I2C_CR1_PE;
	}


void i2c_write(uint8_t slav_add, uint8_t *data, uint8_t length)
	{
	/*Enable I2C*/
		I2C1->CR1 |=I2C_CR1_PE;
		/*Set slave address*/
		I2C1->CR2=(slav_add<<1);
		/*7-bit addressing*/
		I2C1->CR2&=~I2C_CR2_ADD10;
		/*Set number to transfer to length for write operation*/
		I2C1->CR2|=(length<<I2C_CR2_NBYTES_Pos);
		/*Set the mode to write mode*/
		I2C1->CR2&=~I2C_CR2_RD_WRN;
		/*hardware end*/
		I2C1->CR2|=I2C_CR2_AUTOEND;
		/*Generate start*/
		I2C1->CR2|=I2C_CR2_START;
		while(!(I2C1->ISR & I2C_ISR_STOPF))
		{
			/*Check if TX buffer is empty*/
			if(I2C1->ISR & I2C_ISR_TXE)
			{
				/*send memory address*/
				I2C1->TXDR =*data++ ;
			}
		}
		/*Disable I2C*/
		I2C1->CR1 &=~I2C_CR1_PE;

	}

void i2c_write_memory(uint8_t slav_add, uint8_t memadd, uint8_t *data, uint8_t length)
	{

	uint8_t send_arr[length+1];
	send_arr[0]=memadd;
	for (int i=1;(i<length+1);i++)
		{
		send_arr[i]=*data++;
		}

	/*Enable I2C*/
	I2C1->CR1 |=I2C_CR1_PE;
	/*Set slave address*/
	I2C1->CR2=(slav_add<<1);
	/*7-bit addressing*/
	I2C1->CR2&=~I2C_CR2_ADD10;
	/*Set number to transfer to length+1 for write operation*/
	I2C1->CR2|=((length+1)<<I2C_CR2_NBYTES_Pos);
	/*Set the mode to write mode*/
	I2C1->CR2&=~I2C_CR2_RD_WRN;
	/*hardware end*/
	I2C1->CR2|=I2C_CR2_AUTOEND;
	/*Generate start*/
	I2C1->CR2|=I2C_CR2_START;
	int i=0;
	while(!(I2C1->ISR & I2C_ISR_STOPF))
	{
		/*Check if TX buffer is empty*/
		if(I2C1->ISR & I2C_ISR_TXE)
		{
			/*send memory address*/
			I2C1->TXDR =send_arr[i] ;
			i++;
		}
	}
	/*Disable I2C*/
	I2C1->CR1 &=~I2C_CR1_PE;
	}
