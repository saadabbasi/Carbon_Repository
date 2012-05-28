/*
 * CheckerAPI.c
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */


#include "CheckerAPI.h"

void readWireParametersFromIndices(int16_t wire_positions[], WireInfo wireInfo[])
{
	for(int i=0;i<7;i++)
	{
		if(wire_positions[i] != -1)
		{
			readWireParameters(wire_positions[i],&wireInfo[i]);
		}
	}
}

void readWireParameters(uint16_t wire_num, WireInfo *wireInfo)
{
	uint16_t base_address = 32768 + ((wire_num-1) * ROW_WIDTH);
	uint8_t length = 10;
	uint16_t addr_locationA = base_address + 0x10;
	uint16_t addr_locationB = base_address + 0x1A;
	uint16_t addr_colour = base_address + 4;
	uint16_t addr_gauge = base_address + 10;

	eepromRead(wireInfo->locationA,addr_locationA,length);
	eepromRead(wireInfo->locationB,addr_locationB,length);
	eepromRead(wireInfo->colour,addr_colour,COLOUR_WIDTH);
	eepromRead(wireInfo->gauge,addr_gauge,GAUGE_WIDTH);
}

CH_RESULT findFaultsAndReturnFaultyWireInfos(uint8_t board_count, WireInfo faulty_wires[], uint16_t *count)
{
	uint8_t driver;
	uint8_t test_vector[TPOINTS_BYTES], cktInfo[TPOINTS_BYTES], faults[TPOINTS_BYTES];
	uint8_t opens[TPOINTS_BYTES], shorts[TPOINTS_BYTES];
	uint8_t wire;
	int16_t fault_locations[TPOINTS];
	char str[10];

	uint8_t total_bytes = BYTES_PER_BOARD*board_count;

	for(driver = 0; driver < board_count; driver++)
	{
		if(clearVector(driver)!=CH_OK)
		{
			break;
		}
	}

	for(driver = 0; driver < board_count; driver++)
	{
		for(int j = 0; j < board_count; j++)
		{
			if(clearVector(driver)!=CH_OK)
			{
				break;
			}
		}
		if(setFirstBitOnDriver(driver)==CPLD_BOARD_ID_ERR)
		{
			GLCD_SetCursorAddress(160);
			GLCD_WriteText("CPLD_BOARD_ID_ERROR DURING SETTING FIRST BIT.");
			break;
		}
		for(wire=0;wire<72;wire++)
		{
			recieveTestVectorFromConnectedBoards(test_vector, board_count);
			getCKTInfo(wire+driver*TPOINTS_PER_BOARD,total_bytes,cktInfo);
			uint16_t fault_count = detectFaultsAndReturnCount(cktInfo,test_vector,faults,total_bytes);
			uint16_t open_circuit_count = detectOpenCircuitsAndReturnCount(cktInfo,test_vector,opens,total_bytes);
			uint16_t short_circuit_count = detectShortCircuitsAndReturnCount(cktInfo,faults,shorts,total_bytes);

			if(fault_count>0)
			{
				if(short_circuit_count > 0 && open_circuit_count > 0)
				{
					*count = returnFaultyWirePositions(faults,fault_locations,total_bytes);
					readWireParametersFromIndices(fault_locations,faulty_wires);
					return CH_WRONG_SLOT;
				}
				else if(short_circuit_count == 0 && open_circuit_count > 0)
				{
					*count = returnFaultyWirePositions(opens,fault_locations,total_bytes);
					readWireParametersFromIndices(fault_locations,faulty_wires);
					return CH_MISSING_WIRE;
				}
				else if(short_circuit_count > 0 && open_circuit_count == 0)
				{
					uint8_t local_count = 0;
					for(int i=0; i<total_bytes; i++)
					{
						shorts[i] = ~faults[i];
						shorts[i] = shorts[i] & cktInfo[i];
						shorts[i] = ~shorts[i];
						local_count = local_count + countBitsSet(shorts[i]);
					}
					*count = returnFaultyWirePositions(shorts,fault_locations,total_bytes);

					readWireParametersFromIndices(fault_locations,faulty_wires);
					return CH_SHORT_CIRCUIT;
				}
			}
			if(shiftVectorOnDriver(driver)==CPLD_BOARD_ID_ERR)
			{
				GLCD_SetCursorAddress(160);
				GLCD_WriteText("CPLD_BOARD_ID_ERROR DURING SHIFTING VECTOR.");
				break;
			}
		}
	}

	return CH_OK;
}

CH_RESULT getCKTInfo(uint16_t ckt, uint16_t vector_size, char cktInfo[])
{
	//cktInfo[] MUST be 45 bytes in length.
	char str[3];
	uint16_t address = ckt*vector_size;

	eepromRead(cktInfo,address,vector_size);

	//GLCD_SetCursorAddress(400);
	//	for(int i=0;i<vector_size;i++)
	//	{
	//		itoa(cktInfo[i],str,16);
	//		GLCD_WriteText(str);
	//		GLCD_WriteText(" ");
	//	}

	return CH_OK;
}

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

uint8_t returnStoredBoardSequence(void)
{
	char epbuffer[CKT_HEADER_LENGTH] = {0xFF};
	eepromRead(epbuffer,CKT_HEADER_ADDR,CKT_HEADER_LENGTH);

	return	epbuffer[3];
}

void copyEEPROMToFile(void)
{
	FIL file;
	unsigned int bytes_written;
	if(f_open(&file,"/eeprom.bin",FA_READ | FA_WRITE | FA_OPEN_ALWAYS) != FR_OK)
	{

	}

	char buffer[128];
	uint16_t address = 0;

	for(int i=0;i<65536/128;i++)
	{
		eepromRead(buffer,address,128);
		f_write(&file,buffer,128,&bytes_written);
		address = address + 128;
	}

	f_close(&file);
}

CH_RESULT copyCKTFileToEEPROM(void)
{
	uint8_t page = 128;
	uint8_t buffer[128] = {0xFF};
	uint8_t header[CKT_HEADER_LENGTH];

	FIL file;

	for(int i=0;i<128;i++)
	{
		buffer[i] = 0xFF;
	}

	if(f_open(&file,"/map.chk",FA_READ | FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
	{
		GLCD_SetCursorAddress(0);
		GLCD_WriteText("Cannot Open File.");
		return 1;
	}

	uint16_t address = 0;
	unsigned int bytesRead = 0;

	if(f_read(&file,header,CKT_HEADER_LENGTH,&bytesRead)!=FR_OK)
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
	for(int i=0;i<CKT_HEADER_LENGTH;i++)
	{
		buffer[i] = header[i];
	}

	eepromWritePage(buffer,headerAddress);

	do
	{
		if(f_read(&file,buffer,page,&bytesRead)!=FR_OK)
		{
			GLCD_SetCursorAddress(0);
			GLCD_WriteText("Cannot Read File");
			return 2;
		}
		eepromWritePage(buffer,address);
		address = address + page;
		//bytesCopied = bytesCopied + bytesRead;
	}while(bytesRead == 128);

	f_close(&file);
	return CH_OK;
}


CH_RESULT copyLOCFileToEEPROM(void)
{
	FIL f_loc;
	uint8_t header[LOC_HEADER_LENGTH];
	uint8_t buffer[EEPROM_PAGESIZE];
	unsigned int bytesRead;

	if(f_open(&f_loc,"/data.loc",FA_READ | FA_OPEN_EXISTING) != FR_OK)
	{
		return (1);
	}

	if(f_read(&f_loc,header,LOC_HEADER_LENGTH,&bytesRead)!=FR_OK)
	{
		return 2;
	}

	if(header[0] != 'L' || header[1] != 'O' || header[2] != 'C')
	{
		return CH_INVALID_HEADER_LOC;
	}

	for(int i=0;i<LOC_HEADER_LENGTH;i++)
	{
		buffer[i] = header[i];
	}

	eepromWritePage(buffer,LOC_HEADER_ADDR);

	uint16_t address = 0x8000;
	do
	{
		if(f_read(&f_loc,buffer,EEPROM_PAGESIZE,&bytesRead)!=FR_OK)
		{
			GLCD_SetCursorAddress(0);
			GLCD_WriteText("Cannot Read File");
			return 2;
		}
		eepromWritePage(buffer,address);
		address = address + EEPROM_PAGESIZE;
	}while(bytesRead == 128);

	return CH_OK;
}

CH_RESULT verifyLOCFile(void)
{
	FIL f_loc;
	char epbuffer[EEPROM_PAGESIZE];
	char sdbuffer[EEPROM_PAGESIZE];
	unsigned int bytesRead;

	if(f_open(&f_loc,"/data.loc",FA_READ | FA_OPEN_EXISTING) != FR_OK)
	{
		return (1);
	}

	if(f_read(&f_loc,sdbuffer,LOC_HEADER_LENGTH,&bytesRead)!=FR_OK)
	{
		return 2;
	}

	eepromRead(epbuffer,LOC_HEADER_ADDR,LOC_HEADER_LENGTH);
	for(int j=0;j<LOC_HEADER_LENGTH;j++)
	{
		if(sdbuffer[j] != epbuffer[j])
		{
			return CH_VERIFICATION_FAILED;
		}
	}

	uint16_t address = LOC_DATA_ADDR;
	do
	{
		if(f_read(&f_loc,sdbuffer,EEPROM_PAGESIZE,&bytesRead)!=FR_OK)
		{
			return 2;
		}
		eepromRead(epbuffer,address,EEPROM_PAGESIZE);
		for(int j=0;j<bytesRead;j++)
		{
			if(sdbuffer[j] != epbuffer[j])
			{
				return CH_VERIFICATION_FAILED;
			}
		}
		address = address + EEPROM_PAGESIZE;
	}while(bytesRead == EEPROM_PAGESIZE);

	f_close(&f_loc);
	return CH_OK;
}


CH_RESULT programHarness(uint8_t board_count, uint8_t connectedBoards)
{
	int i,j;
	uint8_t header[10];
	uint8_t test_vector[TPOINTS_BYTES];
	unsigned int bytes_written;
	FIL f_map;

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

	for(driver = 0; driver < board_count; driver++)
	{
		if(clearVector(driver)!=CH_OK)
		{
			break;
		}
	}

	for(driver = 0; driver < board_count; driver++)
	{
		for(int j = 0; j < board_count; j++)
		{
			if(clearVector(driver)!=CH_OK)
			{
				break;
			}
		}
		if(setFirstBitOnDriver(driver)==CPLD_BOARD_ID_ERR)
		{
			GLCD_SetCursorAddress(160);
			GLCD_WriteText("CPLD_BOARD_ID_ERROR DURING SETTING FIRST BIT.");
			break;
		}
		for(wire=0;wire<72;wire++)
		{
			recieveTestVectorFromConnectedBoards(test_vector, board_count);
			f_write(&f_map,test_vector,BYTES_PER_BOARD*board_count,&bytes_written);
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

CH_RESULT initalizeDriverCPLDs(uint8_t *board_count, uint8_t *inited_boards)
{
	*inited_boards = 0;
	uint8_t driver;

	for(driver = 0; driver < MAX_DAUGHTER_BOARDS; driver++)
	{
		if(initDriverCPLD(driver) == CPLD_OK)
		{
			*inited_boards |= (1 << driver);
		}
	}

	if(*inited_boards!=1 && *inited_boards!=3 && *inited_boards!=7 && *inited_boards!=15 && *inited_boards!=31)
	{
		return (CH_INVALID_BOARD_SEQUENCE);
	}

	if(*inited_boards == 1)
	{
		*board_count = 1;
	}
	else if(*inited_boards == 3)
	{
		*board_count = 2;
	}
	else if(*inited_boards == 7)
	{
		*board_count = 3;
	}
	else if(*inited_boards == 15)
	{
		*board_count = 4;
	}
	else if(*inited_boards == 31)
	{
		*board_count = 5;
	}
	else
	{
		*board_count = 0;
	}

	return CH_OK;
}

void recieveTestVectorFromConnectedBoards(uint8_t test_vector[], uint8_t board_count)
{
	uint8_t count = 0;
	uint8_t rcv_id = 5;
	while(count < board_count)
	{
		if(recieveTestVectorFromReceiver(rcv_id,test_vector) != CPLD_OK)
		{
			return;
		}
		rcv_id++;
		count++;
	}
}
