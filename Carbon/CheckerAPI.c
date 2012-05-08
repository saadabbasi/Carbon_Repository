/*
 * CheckerAPI.c
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */


#include "CheckerAPI.h"

int8_t returnNumberOfConnectedBoards(void)
{
//	DDRD &= ~(1 << PD7);
//	DDRD &= ~(1 << PD6);
//	DDRD &= ~(1 << PD5);
//	DDRG &= ~(1 << PG0);
//
//	if(PING & _BV(PG0))
//	{
//		return 1;
//	}

	return 0;
}

void recieveTestVectorFromConnectedBoards(uint8_t test_vector[])
{
	recieveTestVectorFromBoard(ControllerBoard,test_vector);
	recieveTestVectorFromBoard(DaughterBoard_One,test_vector);
}
