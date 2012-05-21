/*
 * CheckerAPI.c
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */


#include "CheckerAPI.h"

CH_RESULT checkBoardSequence(uint8_t connectedBoards)
{
	if(connectedBoards!=1 && connectedBoards!=3 && connectedBoards!=7 && connectedBoards!=15 && connectedBoards!=31)
	{
		return (CH_INVALID_BOARD_SEQUENCE);
	}

	return CH_OK;
}

CH_RESULT verifyCKTFile(void)
{
	uint8_t buffer[EEPROM_PAGESIZE] = {0xFF};
	char epbuffer[EEPROM_PAGESIZE] = {0xFF};

	FIL f_circuit_data;
	if(f_open(&f_circuit_data,"/map.chk",FA_READ | FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
	{
		return 1;
	}

	unsigned int bytesRead = 0;
	eepromRead(epbuffer,CKT_HEADER_ADDR,CKT_HEADER_LENGTH);

	if(f_read(&f_circuit_data,buffer,10,&bytesRead)!=FR_OK)
	{
		return 2;
	}

	for(int j=0;j<CKT_HEADER_LENGTH;j++)
	{
		if(buffer[j] != epbuffer[j])
		{
			return CH_VERIFICATION_FAILED;
		}
	}

	uint16_t address = 0;
	do
	{
		if(f_read(&f_circuit_data,buffer,128,&bytesRead)!=FR_OK)
		{
			return 2;
		}
		eepromRead(epbuffer,address,EEPROM_PAGESIZE);
		for(int j=0;j<bytesRead;j++)
		{
			if(buffer[j] != epbuffer[j])
			{
				return CH_VERIFICATION_FAILED;
			}
		}
		address = address + EEPROM_PAGESIZE;
	}while(bytesRead == EEPROM_PAGESIZE);

	return CH_OK;
}

CH_RESULT copyCKTFileToEEPROM(FIL *file)
{
	uint8_t page = 128;
	uint8_t buffer[128] = {0xFF};
	uint8_t header[10];

	for(int i=0;i<128;i++)
	{
		buffer[i] = 0xFF;
	}

	if(f_open(file,"/map.chk",FA_READ | FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
	{
		GLCD_SetCursorAddress(0);
		GLCD_WriteText("Cannot Open File.");
		return 1;
	}

	uint16_t address = 0;
	unsigned int bytesRead = 0;

	if(f_read(file,header,sizeof(header),&bytesRead)!=FR_OK)
	{
		return 2;
	}

	if(header[0] != 'C' || header[1] != 'K' || header[2] != 'T')
	{
		return CH_INVALID_HEADER_CKT;
	}

	if(checkBoardSequence(header[3])!=CH_OK)
	{
		return CH_INVALID_BOARD_SEQUENCE;
	}

	uint16_t headerAddress = CKT_HEADER_ADDR;
	for(int i=0;i<10;i++)
	{
		buffer[i] = header[i];
	}

	eepromWritePage(buffer,headerAddress);

	do
	{
		if(f_read(file,buffer,page,&bytesRead)!=FR_OK)
		{
			GLCD_SetCursorAddress(0);
			GLCD_WriteText("Cannot Read File");
			return 2;
		}
		eepromWritePage(buffer,address);
		address = address + page;
		//bytesCopied = bytesCopied + bytesRead;
	}while(bytesRead == 128);

	f_close(file);
	return CH_OK;
}

CH_RESULT programHarness(void)
{
	int i,j;
	uint8_t header[10];
	uint8_t test_vector[TPOINTS_BYTES];
	unsigned int bytes_written;
	FIL f_map;

	uint8_t connectedBoards = initalizeDriverCPLDs();
	if(connectedBoards!=1 && connectedBoards!=3 && connectedBoards!=7 && connectedBoards!=15 && connectedBoards!=31)
	{
		return (CH_INVALID_BOARD_SEQUENCE);
	}

	if(f_open(&f_map,"/MAP.CHK",FA_READ | FA_WRITE | FA_OPEN_ALWAYS) != FR_OK)
	{
		return (1);
	}

	/* As driver IDs (see SPIController.h) start from 0 and end at 4, the for loop
	 * uses the integer values directly to set and shift the CPLDs. This helps
	 * avoid repeated code if the driver is referenced directly, like so "B0_Driver".
	 */

	header[0] = 'C'; header[1] = 'K'; header[2] = 'T'; // "CKT" file identifier.
	header[3] =	connectedBoards; // least 5 significant bits represent the boards connected.

	for(int i=4; i<10; i++)
	{
		header[i] = 0xFF;
	}


	uint8_t driver, wire;
	f_write(&f_map,header,sizeof(header),&bytes_written);
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
			//f_sync(&f_map);
			//bytes_written = 100;
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
	return (CH_OK);
}

uint8_t initalizeDriverCPLDs(void)
{
	uint8_t initedBoards = 0;
	uint8_t driver;

	for(driver = 0; driver < 5; driver++)
	{
		if(initDriverCPLD(driver) == CPLD_OK)
		{
			initedBoards |= (1 << driver);
		}
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
