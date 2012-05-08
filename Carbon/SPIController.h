/*
 * SPIController.h
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#ifndef SPICONTROLLER_H_
#define SPICONTROLLER_H_

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define SCK (1 << PB1)
#define MOSI (1 << PB2)
#define MISO (1 << PB3)

#define	SDCard_p				(1 << PB0)
#define B0_Driver_p				(1 << PF1)
#define	B0_Rcv_p				(1 << PB7)
#define B1_Driver_p				(1 << PE6)
#define B1_Rcv_p				(1 << PE7)
#define B2_Driver_p				(1 << PE4)
#define B2_Rcv_p				(1 << PE5)
#define B3_Driver_p				(1 << PE2)
#define B3_Rcv_p				(1 << PE3)
#define B4_Driver_p				(1 << PE0)
#define B4_Rcv_p				(1 << PE1)
#define EEPROM_p				(1 << PG5)
#define	DESELECT_p				0x00

typedef enum
{
	SDCard,
	B0_Driver,
	B0_Rcv,
	B1_Driver,
	B1_Rcv,
	B2_Driver,
	B2_Rcv,
	B3_Driver,
	B3_Rcv,
	B4_Driver,
	B4_Rcv,
	EEPROM,
	DESELECT
} SPIDevice_t;

void initSPI(void);
void setupSPIPorts(void);
void writeByte(uint8_t byte);
uint8_t readByte(void);
void selectDevice(SPIDevice_t device);

#endif /* SPICONTROLLER_H_ */
