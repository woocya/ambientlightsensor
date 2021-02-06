#include "i2c_functions.h"

void stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);  //Transmit STOP condition
}

uint8_t start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Send START condition
	while (!(TWCR & (1<<TWINT)));  //Wait for TWINT Flag set. This indicates that the START condition has been transmitted
	if ((TWSR & 0xF8) != TW_START)    //Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
		return TW_BUS_ERROR;
	
	return TW_MT_START_OK;
}

uint8_t write(uint8_t reg, uint8_t data, uint8_t error_data)
{
	start();
	
	TWDR = SLA_W;   //   SLA_W            //Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));  //Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK)  //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
		return TW_MT_SLA_NACK;

	TWDR = reg;       //Load code/register address into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));  //Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)  //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
		return TW_MT_DATA_NACK;
	
	if(error_data!=0)
	{
		TWDR = data;   //   SLA_W            //Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
		TWCR = (1<<TWINT) | (1<<TWEN);
		while (!(TWCR & (1<<TWINT)));  //Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
		if ((TWSR & 0xF8) != TW_MT_DATA_ACK)  //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
			return error_data;
	}
	
	stop();
	
	return TW_MT_WRITE_OK;
}

uint8_t read(uint8_t *array)
{
	TCNT1=0x0000;
	start();
	TWDR = SLA_R;   //   read           //Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));  //Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received.
	
	if ((TWSR & 0xF8) == TW_MR_SLA_ACK)  //Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
	{
		TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		while (!(TWCR & (1<<TWINT)));
		
		if ((TWSR & 0xF8) == TW_MR_DATA_ACK)
		{
			array[0]=TWDR;
			TWCR = (1<<TWINT) | (1<<TWEN);
			while (!(TWCR & (1<<TWINT)));
			if ((TWSR & 0xF8) == TW_MR_DATA_NACK)
			{
				array[1]=TWDR;
				stop();
				return TW_MR_DATA_OK;
			}
			else return TW_MR_DATA_ERROR;
		}
		else return TW_MR_DATA_NACK;
	}	
	else return TW_MR_SLA_NACK;
}
