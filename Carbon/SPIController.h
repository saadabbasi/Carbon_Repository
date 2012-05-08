/*
 * SPIController.h
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#ifndef SPICONTROLLER_H_
#define SPICONTROLLER_H_

typedef enum
{
	SDCard = (1 << PB0),
	B0_Driver = (1 << PF1),
	B0_Rcv = (1 << PB7),
	B1_Driver =(1 << PE6),
	B1_Rcv = (1 << PE7),
	B2_Driver = (1 << PE4),
	B2_Rcv = (1 << PE5),
	B3_Driver = (1 << PE2),
	B3_Rcv = (1 << PE3),
	B4_Driver = (1 << PE0),
	B4_Rcv = (1 << PE1),
	EEPROM = (1 << PG5),
	DESELECT = 0x00
} SPIDevice_t;

void initSPI(void);
void setupSPIPorts(void);
void writeByte(uint8_t byte);
uint8_t readByte(void);
void selectChip(uint8_t chip);

#endif /* SPICONTROLLER_H_ */
