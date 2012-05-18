/*
 * CPLD_API.h
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#ifndef CPLD_API_H_
#define CPLD_API_H_

#include "SPIController.h"
#include <stdint.h>

#define TESTPOINTS_PER_BOARD	72

typedef enum
{
	CPLD_OK = 0,
	CPLD_INIT_ERR,
	CPLD_BOARD_ID_ERR,
} CPLDRESULT;

typedef enum
{
	ControllerBoard,
	DaughterBoard_One,
	DaughterBoard_Two,
	DaughterBoard_Three,
	DaughterBoard_Four
} IO_Board;

CPLDRESULT deviceIsADriver(SPIDevice_t device);
CPLDRESULT deviceIsAReciever(SPIDevice_t device);
CPLDRESULT initDriverCPLD(SPIDevice_t device);
CPLDRESULT recieveTestVectorFromReceiver(SPIDevice_t device, uint8_t testVector[]); // returns a 72 point test vector from specified receiver CPLD.
CPLDRESULT setFirstBitOnDriver(SPIDevice_t device);
CPLDRESULT shiftVectorOnDriver(SPIDevice_t device);
uint8_t getSetBitPositionFromBoard(IO_Board board);

#endif /* CPLD_API_H_ */
