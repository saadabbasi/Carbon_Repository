/*
 * CPLD_API.h
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#ifndef CPLD_API_H_
#define CPLD_API_H_

#include "SPIController.h"

#define TESTPOINTS_PER_BOARD	72

typedef enum
{
	ControllerBoard,
	DaughterBoard_One,
	DaughterBoard_Two,
	DaughterBoard_Three,
	DaughterBoard_Four
} IO_Board;

void recieveTestVectorFromBoard(IO_Board board, uint8_t testVector[]); // returns a 72 point test vector from specified daughterboard
void setFirstBitOnBoard(IO_Board board);
void shiftVectorOnBoard(IO_Board board);
uint8_t getSetBitPositionFromBoard(IO_Board board);

#endif /* CPLD_API_H_ */
