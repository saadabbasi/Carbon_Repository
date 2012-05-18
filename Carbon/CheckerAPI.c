/*
 * CheckerAPI.c
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */


#include "CheckerAPI.h"

CH_RESULT programHarness(void)
{
	int i,j;
	uint8_t test_vector[TPOINTS_BYTES];
	unsigned int bytes_written;
	FIL f_map;

	uint8_t connectedBoards = initalizeDriverCPLDs();
	if(connectedBoards!=1 && connectedBoards!=3 && connectedBoards!=7 && connectedBoards!=15 && connectedBoards!=31)
	{
		return (INVALID_BOARD_SEQUENCE);
	}

	if(f_open(&f_map,"/MAP.CHK",FA_READ | FA_WRITE | FA_OPEN_ALWAYS) != FR_OK)
	{
		return (1);
	}

	uint8_t driver, wire;
	for(driver = 0; driver < 5; driver++)
	{
		if(setFirstBitOnDriver(driver)==CPLD_BOARD_ID_ERR)
		{
			GLCD_SetCursorAddress(160);
			GLCD_WriteText("CPLD_BOARD_ID_ERROR DURING SETTING FIRST BIT.");
			break;
		}
		for(wire=0;wire<72;wire++)
		{
			recieveTestVectorFromConnectedBoards(test_vector);
			f_write(&f_map,test_vector,TPOINTS_BYTES,&bytes_written);
			if(bytes_written == 0)
			{
				return 1;
			}
			if(shiftVectorOnDriver(driver)==CPLD_BOARD_ID_ERR)
			{
				GLCD_SetCursorAddress(160);
				GLCD_WriteText("CPLD_BOARD_ID_ERROR DURING SHIFTING VECTOR.");
				break;
			}
		}
	}

	f_close(&f_map);
//
//	setFirstBitOnDriver(ControllerBoard);
//	for(i=0;i<72;i++)
//	{
//		recieveTestVectorFromConnectedBoards(test_vector);
//
//		f_write(&f_map,test_vector,TPOINTS_BYTES,&bytes_written);
//		if(bytes_written == 0)
//		{
//			return (1);
//		}
//		shiftVectorOnBoard(ControllerBoard);
//	}
//
//	setFirstBitOnDriver(DaughterBoard_One);
//	for(i=0;i<72;i++)
//	{
//		recieveTestVectorFromConnectedBoards(test_vector);
//
//		f_write(&f_map,test_vector,TPOINTS_BYTES,&bytes_written);
//		if(bytes_written == 0)
//		{
//			return (1);
//		}
//		shiftVectorOnBoard(DaughterBoard_One);
//	}
//
//	setFirstBitOnDriver(DaughterBoard_Two);
//	for(i=0;i<72;i++)
//	{
//		recieveTestVectorFromConnectedBoards(test_vector);
//
//		f_write(&f_map,test_vector,TPOINTS_BYTES,&bytes_written);
//		if(bytes_written == 0)
//		{
//			return (1);
//		}
//		shiftVectorOnBoard(DaughterBoard_Two);
//	}
//
//	setFirstBitOnDriver(DaughterBoard_Three);
//	for(i=0;i<72;i++)
//	{
//		recieveTestVectorFromConnectedBoards(test_vector);
//
//		f_write(&f_map,test_vector,TPOINTS_BYTES,&bytes_written);
//		if(bytes_written == 0)
//		{
//			return (1);
//		}
//		shiftVectorOnBoard(DaughterBoard_Three);
//	}
//
//	setFirstBitOnDriver(DaughterBoard_Four);
//	for(i=0;i<72;i++)
//	{
//		recieveTestVectorFromConnectedBoards(test_vector);
//
//		f_write(&f_map,test_vector,TPOINTS_BYTES,&bytes_written);
//		if(bytes_written == 0)
//		{
//			return (1);
//		}
//		shiftVectorOnBoard(DaughterBoard_Four);
//
//	}
//
//	f_close(&f_map);

	return (CH_OK);
}

uint8_t initalizeDriverCPLDs(void)
{
	uint8_t initedBoards = 0;

	if(initDriverCPLD(B0_Driver)==CPLD_OK)
	{
		initedBoards |= (1 << 0);
	}

	if(initDriverCPLD(B1_Driver)==CPLD_OK)
	{
		initedBoards |= (1 << 1);
	}

	if(initDriverCPLD(B2_Driver)==CPLD_OK)
	{
		initedBoards |= (1 << 2);
	}

	if(initDriverCPLD(B3_Driver)==CPLD_OK)
	{
		initedBoards |= (1 << 3);
	}

	if(initDriverCPLD(B4_Driver)==CPLD_OK)
	{
		initedBoards |= (1 << 4);
	}

	return (initedBoards);
}

void recieveTestVectorFromConnectedBoards(uint8_t test_vector[])
{
	if(recieveTestVectorFromReceiver(B0_Rcv,test_vector) != CPLD_OK)
	{
		return;
	}
	if(recieveTestVectorFromReceiver(B1_Rcv,test_vector) != CPLD_OK)
	{
		return;
	}
	if(recieveTestVectorFromReceiver(B2_Rcv,test_vector) != CPLD_OK)
	{
		return;
	}
	if(recieveTestVectorFromReceiver(B3_Rcv,test_vector) != CPLD_OK)
	{
		return;
	}
	if(recieveTestVectorFromReceiver(B4_Rcv,test_vector) != CPLD_OK)
	{
		return;
	}
}
