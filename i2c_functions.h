#ifndef I2C_FUNCTIONS_H_
#define I2C_FUNCTIONS_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/twi.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include <time.h>
#include <math.h>


#define TW_MT_WRITE_OK 0xFF
#define TW_MT_START_OK 0xFF
#define TW_MR_DATA_OK 0xFF
#define TW_MR_DATA_ERROR 0x01
#define SLA_W 0b01000110
#define SLA_R 0b01000111
#define POWER_DOWN 0b00000111
#define MODE_CODE 0b00010000



void stop();
uint8_t start();
uint8_t write(uint8_t reg, uint8_t data, uint8_t error_data);
uint8_t read(uint8_t *array);


#endif /* I2C_FUNCTIONS_H_ */
