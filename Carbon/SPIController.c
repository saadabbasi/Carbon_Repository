/*
 * SPIController.c
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#include "SPIController.h"


void initSPI(void)
{
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << CPHA) | (0 << CPOL) | (0 << SPR1) | (0 << SPR0);
	SPSR = (0 << SPI2X);
	_delay_us(1);
}
void setupSPIPorts(void)
{
	PORTB = 0b11111101;
	DDRB = (MOSI) | (SCK) | (B0_Rcv_p) | (SDCard_p);


	DDRE |= 0xFF;
	PORTE |= 0xFF;

	DDRG |= (1 << PG5);
	PORTG |= (1 << PG5);

	DDRF |= (1 << PF1);
	PORTF |= (1 << PF1);

	selectDevice(DESELECT);
}
void writeByte(uint8_t byte)
{
	uint8_t byteFromSlave;
	SPDR = byte;
	while(!(SPSR & (1 << SPIF)));
	byteFromSlave = SPDR;
}

uint8_t readByte(void)
{
	uint8_t byteFromSlave;
	SPDR = 0xFF;
	while(!(SPSR & (1 << SPIF)));
	byteFromSlave = SPDR;
	return byteFromSlave;
}
void selectDevice(SPIDevice_t device)
{
	PORTE = 0xFF;
	PORTB = SDCard_p | B0_Rcv_p;
	PORTF |= B0_Driver_p;
	PORTG |= EEPROM_p;

	if(device == EEPROM)
	{
		PORTG &= ~EEPROM_p;
	}
	else if(device == B0_Driver)
	{
		PORTF &= ~B0_Driver_p;
	}
	else if(device == B0_Rcv)
	{
		PORTB &= ~B0_Rcv_p;
	}
	else if(device == B1_Driver)
	{
		PORTE &= ~B1_Driver_p;
	}
	else if(device == B1_Rcv)
	{
		PORTE &= ~B1_Rcv_p;
	}
	else if(device == B2_Driver)
	{
		PORTE &= ~B2_Driver_p;
	}
	else if(device == B2_Rcv)
	{
		PORTE &= ~B2_Rcv_p;
	}
	else if(device == B3_Driver)
	{
		PORTE &= ~B3_Driver_p;
	}
	else if(device == B3_Rcv)
	{
		PORTE &= ~B3_Rcv_p;
	}
	else if(device == B4_Driver)
	{
		PORTE &= ~B4_Driver_p;
	}
	else if(device == B4_Rcv)
	{
		PORTE &= ~B4_Rcv_p;
	}

}
