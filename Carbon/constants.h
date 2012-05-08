/*
 * constants.h
 *
 *  Created on: Apr 2, 2012
 *      Author: saad
 */

#include <avr/io.h>

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define TESTPOINTS				72
#define	BYTE_WIDTH				8

#define COLOUR					0
#define	GAUGE					6
#define LOCATION_A				12
#define	LOCATION_B				22

#define COLOUR_WIDTH			6
#define	GAUGE_WIDTH				6
#define	LOCATION_WIDTH			10
#define ROW_WIDTH				36

#define SCK						(1 << PB1)
#define	MOSI					(1 << PB2)
#define MISO					(1 << PB3)

// SPI Peripherals
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


#endif /* CONSTANTS_H_ */


