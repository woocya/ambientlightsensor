#include "i2c_functions.h"

void uart_config()
{
	UCSR0B |= 0b10011000;
	UCSR0C = 0x0E;
	UBRR0L = 103;
}

void wysylanko(char* array)
{
	int len=strlen(array);
	int cntr;
	for (cntr=0; (*(array+cntr)!='\0' && cntr<len); cntr++)
	{
		while ((UCSR0A & 32) == 0);
		UDR0 = *(array+cntr);
	}
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}


int main(void)
{
	char number[20];
	uint8_t  bytes[2];
	bytes[0]=0;
	bytes[1]=0;
	uint16_t value=0;
	uart_config();

	TWSR |= (1<<TWPS0); // SCL
	TWBR = 0x03;
	
    write(MODE_CODE, 0, 0);
	_delay_ms(180);
	
	for (int a=0;a<1023;a=a+2)
	{
		read(bytes);	
			
		value = (((bytes[0] & 0xff) << 8) | (bytes[1] & 0xff)); 
		value = value / 1.2;  // divide with rounding
		bytes[0] = value >> 8;     // high byte (0x12)
		bytes[1] = value & 0x00FF; // low byte (0x34)
		
		sprintf(number, "%u", value);
		wysylanko(number);
		wysylanko(" lx\n");
		
		EEPROM_write(a, bytes[0]);
		EEPROM_write(a+1, bytes[1]);
		_delay_ms(115);
	}
	
	write(POWER_DOWN, 0, 0);
    while (1) 
    {
    }
}
