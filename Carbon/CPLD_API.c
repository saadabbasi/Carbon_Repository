/*
 * CPLD_API.c
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#include "CPLD_API.h"

void recieveTestVectorFromBoard(IO_Board board, uint8_t testVector[]) // returns a 72 point test vector from specified daughterboard
{
	int starting_position = 0;
	int ending_position = 9;
	if(board == ControllerBoard)
	{
		selectDevice(B0_Rcv);
		starting_position = 9;
		ending_position = 18;
	}
	else if(board == DaughterBoard_One)
	{

		selectDevice(B1_Rcv);
		starting_position = 0;
		ending_position = 9;
	}
	else if(board == DaughterBoard_Two)
	{
		selectDevice(B2_Rcv);
	}
	else if(board == DaughterBoard_Three)
	{
		selectDevice(B3_Rcv);
	}
	else if(board == DaughterBoard_Four)
	{
		selectDevice(B4_Rcv);
	}
	else
	{
		return;
	}

	for(int i=starting_position;i<ending_position;i++)
	{
		testVector[i] = readByte();
	}
	selectDevice(DESELECT);
}

void setFirstBitOnBoard(IO_Board board)
{
	if(board == ControllerBoard)
	{
		selectDevice(B0_Driver);
	}
	else if(board == DaughterBoard_One)
	{
		selectDevice(B1_Driver);
	}
	else if(board == DaughterBoard_Two)
	{
		selectDevice(B2_Driver);
	}
	else if(board == DaughterBoard_Three)
	{
		selectDevice(B3_Driver);
	}
	else if(board == DaughterBoard_Four)
	{
		selectDevice(B4_Driver);
	}
	else
	{
		return;
	}
	writeByte(0x01);
	selectDevice(DESELECT);
}

void shiftVectorOnBoard(IO_Board board)
{
	if(board == ControllerBoard)
	{
		selectDevice(B0_Driver);
	}
	else if(board == DaughterBoard_One)
	{
		selectDevice(B1_Driver);
	}
	else if(board == DaughterBoard_Two)
	{
		selectDevice(B2_Driver);
	}
	else if(board == DaughterBoard_Three)
	{
		selectDevice(B3_Driver);
	}
	else if(board == DaughterBoard_Four)
	{
		selectDevice(B4_Driver);
	}
	else
	{
		return;
	}
	writeByte(0xAA);
	selectDevice(DESELECT);
}
