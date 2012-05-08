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
	DDRB = (MOSI) | (SCK) | (B0_Rcv) | (SDCard);


	DDRE = 0xFF;
	PORTE = 0xFF;

	DDRG = (1 << PG5);
	PORTG = (1 << PG5);

	chipSelect(DESELECT);
}
void writeByte(uint8_t byte)
{

}
uint8_t readByte(void)
{
	uint8_t byteFromSlave;
	SPDR = 0xFF;
	while(!(SPSR & (1 << SPIF)));
	byteFromSlave = SPDR;
	return byteFromSlave;
}
void selectChip(SPIDevice_t device)
{

}
