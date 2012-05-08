/*
 * harnessio.c
 *
 *  Created on: Apr 2, 2012
 *      Author: saad
 *
 *     This file contains functions for high level access to the EEPROM and SD Card.
 *
 *
 */

#include "harnessio.h"
#include "macros.h"

uint8_t verifyHarnessCircuitData(void)
{
	uint8_t buffer[EEPROM_PAGESIZE] = {0xFF};
	char epbuffer[EEPROM_PAGESIZE] = {0xFF};
	
	FIL f_circuit_data;
	if(f_open(&f_circuit_data,"/map.chk",FA_READ | FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
	{
		return 1;
	}
	
	uint16_t address = 0;
	unsigned int bytesRead = 0;
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
				return 3;
			}
		}
		address = address + EEPROM_PAGESIZE;
	}while(bytesRead == EEPROM_PAGESIZE);

	return 0;
}

uint8_t copyHarnessCircuitData(void)
{
	uint8_t page = 128;
	uint8_t buffer[128] = {0xFF};
	
	FIL f_circuit_data;
	if(f_open(&f_circuit_data,"/map.chk",FA_READ | FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
	{
		GLCD_SetCursorAddress(0);
		GLCD_WriteText("Cannot Open File.");
		return 1;
	}
	
	
	uint16_t address = 0;
	unsigned int bytesRead = 0;
	do
	{
		if(f_read(&f_circuit_data,buffer,page,&bytesRead)!=FR_OK)
		{
			GLCD_SetCursorAddress(0);
			GLCD_WriteText("Cannot Read File");
			return 2;
		}
		writeBuffer(buffer,address);
		address = address + page;
	}while(bytesRead == 128);

	f_close(&f_circuit_data);
	f_mount(0,0);
	return 0;
}

void readWireParametersFromIndices(uint16_t wire_positions[], WireInfo wireInfo[])
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
	uint16_t base_address = 32768 + (wire_num * ROW_WIDTH);
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

void returnWireInfo(FIL *f_ptr, int16_t wire_position[], WireInfo wire[])
{
	fetchWireInfo(f_ptr,wire_position[0]+1,wire[0].colour, COLOUR, COLOUR_WIDTH);
	fetchWireInfo(f_ptr,wire_position[0]+1,wire[0].gauge, GAUGE, GAUGE_WIDTH);
	fetchWireInfo(f_ptr,wire_position[0]+1,wire[0].locationA, LOCATION_A, LOCATION_WIDTH);
	fetchWireInfo(f_ptr,wire_position[0]+1,wire[0].locationB, LOCATION_B, LOCATION_WIDTH);

	for(int i=1;i<7;i++)
	{
		if(wire_position[i]!=-1)
		{
			fetchWireInfo(f_ptr,wire_position[i]+1,wire[i].colour, COLOUR, COLOUR_WIDTH);
			fetchWireInfo(f_ptr,wire_position[i]+1,wire[i].gauge, GAUGE, GAUGE_WIDTH);
			fetchWireInfo(f_ptr,wire_position[i]+1,wire[i].locationA, LOCATION_A, LOCATION_WIDTH);
			fetchWireInfo(f_ptr,wire_position[i]+1,wire[i].locationB, LOCATION_B, LOCATION_WIDTH);
		}
	}
}

void getHarnessName(FIL *f_ptr, char harness_name[])
{
	DWORD f_size = f_size(f_ptr);

	unsigned int bytesRead;
	f_lseek(f_ptr,f_size - 15);
	f_read(f_ptr,harness_name,15,&bytesRead);
}


void getWireInfo(FIL *f_ptr, int wire_num, WireInfo *wire)
{
	fetchWireInfo(f_ptr,wire_num,wire->colour, COLOUR, COLOUR_WIDTH);
	fetchWireInfo(f_ptr,wire_num,wire->gauge, GAUGE, GAUGE_WIDTH);
	fetchWireInfo(f_ptr,wire_num,wire->locationA, LOCATION_A, LOCATION_WIDTH);
	fetchWireInfo(f_ptr,wire_num,wire->locationB, LOCATION_B, LOCATION_WIDTH);
}

void fetchWireInfo(FIL *f_ptr, int wire_num, char wire_info[], int parameter, int width)
{
	unsigned int bytesRead;
	int wire;

	if(f_lseek(f_ptr,(wire_num - 1)*ROW_WIDTH + 2)==FR_OK)
	{
		f_read(f_ptr,&wire,sizeof(wire),&bytesRead);
		if(wire_num == wire)
		{
			f_lseek(f_ptr,f_tell(f_ptr) + parameter);
			f_read(f_ptr,wire_info,width,&bytesRead);
		}
	}
}

uint8_t getSetBitPosition(void)
{
	uint8_t set_bit_position;
	selectDevice(B0_Driver);
	writeByte(CMD_GETSETBITPOSITION);
	selectDevice(DESELECT);
	selectDevice(B0_Driver);
	set_bit_position = readByte();
	selectDevice(DESELECT);
	return set_bit_position;
}

void recieveTestVector(uint8_t testVector[])
{
	selectDevice(B0_Rcv);
	for(int i=0;i<TESTPOINTS/8;i++)
	{
		testVector[i] = readByte();
	}
	selectDevice(DESELECT);
}

void read_ckt_data(FIL *f_ptr, int ckt_num, unsigned char ckt_data[])
{
	unsigned int bytesRead;

	if(f_lseek(f_ptr,(ckt_num)*9)==FR_OK)
	{
		f_read(f_ptr,ckt_data,9,&bytesRead);
	}
}

void setFirstBit(void)
{
	selectDevice(B0_Driver);
	writeByte(CMD_SETFIRSTBIT);
	selectDevice(DESELECT);
}

void shiftLeft(void)
{
	selectDevice(B0_Driver);
	writeByte(CMD_SHIFTLEFT);
	selectDevice(DESELECT);
}

