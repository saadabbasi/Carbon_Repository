/*
 * CPLD_API.c
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#include "CPLD_API.h"

CPLDRESULT deviceIsADriver(SPIDevice_t device)
{
	if(device!=B0_Driver && device!=B1_Driver && device!=B2_Driver && device!=B3_Driver && device!=B4_Driver)
	{
		return (CPLD_BOARD_ID_ERR);
	}
	return CPLD_OK;
}

CPLDRESULT deviceIsAReciever(SPIDevice_t device)
{
	if(device!=B0_Rcv && device!=B1_Rcv && device!=B2_Rcv && device!=B3_Rcv && device!=B4_Rcv)
	{
		return (CPLD_BOARD_ID_ERR);
	}
	return CPLD_OK;
}

CPLDRESULT initDriverCPLD(SPIDevice_t device)
{
	if(deviceIsADriver(device) == CPLD_BOARD_ID_ERR)
	{
		return CPLD_BOARD_ID_ERR;
	}

	selectDevice(device);
	writeByte(0x11);
	selectDevice(DESELECT);
	selectDevice(device);
	uint8_t initByte = readByte();
	selectDevice(DESELECT);
	if(initByte!=72)
	{
		return (CPLD_INIT_ERR);
	}

	return (CPLD_OK);
}

CPLDRESULT recieveTestVectorFromReceiver(SPIDevice_t device, uint8_t testVector[]) // returns a 72 point test vector from specified daughterboard
{
	int starting_position = 0;
	int ending_position = 9;
	if(device == B0_Rcv)
	{
		selectDevice(B0_Rcv);
		starting_position = 0;
		ending_position = 9;
	}
	else if(device == B1_Rcv)
	{
		selectDevice(B1_Rcv);
		starting_position = 9;
		ending_position = 18;
	}
	else if(device == B2_Rcv)
	{
		selectDevice(B2_Rcv);
		starting_position = 18;
		ending_position = 27;
	}
	else if(device == B3_Rcv)
	{
		selectDevice(B3_Rcv);
		starting_position = 27;
		ending_position = 36;
	}
	else if(device == B4_Rcv)
	{
		selectDevice(B4_Rcv);
		starting_position = 36;
		ending_position = 45;
	}
	else
	{
		return CPLD_BOARD_ID_ERR;
	}

	for(int i=starting_position;i<ending_position;i++)
	{
		testVector[i] = readByte();
	}
	selectDevice(DESELECT);
	return CPLD_OK;
}

CPLDRESULT setFirstBitOnDriver(SPIDevice_t device)
{
	if(deviceIsADriver(device) == CPLD_BOARD_ID_ERR)
	{
		return CPLD_BOARD_ID_ERR;
	}
	selectDevice(device);
	writeByte(0x01);
	selectDevice(DESELECT);
	return CPLD_OK;
}

CPLDRESULT shiftVectorOnDriver(SPIDevice_t device)
{
	if(deviceIsADriver(device) == CPLD_BOARD_ID_ERR)
	{
		return CPLD_BOARD_ID_ERR;
	}
	selectDevice(device);
	writeByte(0xAA);
	selectDevice(DESELECT);
	return CPLD_OK;
}
