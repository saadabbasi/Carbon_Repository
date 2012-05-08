/*
 * SPIController.h
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#ifndef SPICONTROLLER_H_
#define SPICONTROLLER_H_

#define	SDCard					1
#define B0_Driver				2
#define	B0_Rcv					3
#define B1_Driver				4
#define B1_Rcv					5
#define B2_Driver				6
#define B2_Rcv					7
#define B3_Driver				8
#define B3_Rcv					9
#define B4_Driver				10
#define B4_Rcv					11
#define EEPROM					12
#define	DESELECT				0x00

typedef enum
{	SDCard,
	B0_Driver,
	B0_Receiver,
	B1_Driver,
	B1_Receiver,
	B2_Driver,
	B2_Receiver,
	B3_Driver,
	B3_Receiver,
	B4_Driver,
	B4_Receiver,
	EEPROM,
	DESELECT} SPIDevice_t;

void initSPI(void);
void setupSPIPorts(void);
void writeByte(uint8_t byte);
uint8_t readByte(void);
void selectChip(SPIDevice_t device);

#endif /* SPICONTROLLER_H_ */
