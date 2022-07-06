/*
 * i2c.h
 *
 *  Created on: Jul 5, 2022
 *      Author: hussamaldean
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"
#include "stm32l0xx.h"

void i2c_init(uint32_t timing);
void i2_read(uint8_t slav_add, uint8_t memadd, uint8_t *data, uint8_t length );
void i2c_write(uint8_t slav_add, uint8_t *data, uint8_t length);
void i2c_write_memory(uint8_t slav_add, uint8_t memadd, uint8_t *data, uint8_t length);
#endif /* I2C_H_ */
