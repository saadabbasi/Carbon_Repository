#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "uart.h"
#include "xitoa.h"
#include "ff.h"
#include "diskio.h"
#include "rtc.h"
#include "sed1335.h"
#include "25LC512.h"
#include "constants.h"
#include "harnessio.h"
#include "SPIController.h"
#include "CPLD_API.h"
#include "CheckerAPI.h"

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

#define CHAR_BIT 8
#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

#define CMD_SHIFTLEFT 0xAA
#define CMD_ALLZEROES 0x0F
#define CMD_SETFIRSTBIT 0x01
#define CMD_GETSETBITPOSITION 0xF0

#define ProFont20Pt				20;
#define ProFont48Pt				48;

/*----------------------CONSTANTS-------------------------------*/
#define MAX_TEST_POINTS			32
#define NUM_BYTES               4;
//FUSES = {0xAF, 0xC3, 0xFF};	/* Fuse values: Low, High, Ext */
/* This is the fuse settings for this project. The fuse data will be output into the
 hex file with program code. However some flash programmers may not support this
 sort of hex files. If it is the case, use these values to program the fuse bits.
 */

DWORD AccSize; /* Work register for fs command */
WORD AccFiles, AccDirs;
FILINFO Finfo;
#if _USE_LFN
char Lfname[_MAX_LFN+1];
#endif

char Line[100]; /* Console input buffer */

FATFS Fatfs[_VOLUMES]; /* File system object for each logical drive */
FIL File[2]; /* File object */

BYTE Buff[2048]; /* Working buffer */

volatile WORD Timer; /* 100Hz increment timer */

#if _MULTI_PARTITION != 0
const PARTITION Drives[] = { {0,0}, {0,1}};
#endif

/*---------------------------------------------------------*/
/* 100Hz timer interrupt generated by OC2                  */
/*---------------------------------------------------------*/

ISR(TIMER2_COMP_vect) {
	Timer++; /* Performance counter for this module */
	disk_timerproc(); /* Drive timer procedure of low level disk I/O module */
}

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */

DWORD get_fattime(void) {
#ifdef RTC_DEFINED
	RTC rtc;

	/* Get local time */
	rtc_gettime(&rtc);

	/* Pack date and time into a DWORD variable */
	return ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
#else
	return 0;
#endif
}

uint8_t learnHarness(void);

int main(void) {
	FATFS FileSystemObject;
	GLCD_Initialize();

	setupSPIPorts();

	uint8_t eeprom_buffer[128];

	if (f_mount(0, &FileSystemObject) != FR_OK) {
		return 0;
	}

	unsigned int bytesRead;
	uint16_t address = 32768;
	initSPI();

	/*do
	 {
	 if(f_read(&f_harness,eeprom_buffer,128,&bytesRead)!=FR_OK)
	 {
	 GLCD_SetCursorAddress(0);
	 GLCD_WriteText("Cannot Read File");
	 return 0;
	 }
	 writeBuffer(eeprom_buffer,address);
	 address = address + 128;
	 }while(bytesRead == 128);

	 f_close(&f_harness);
	 f_mount(0,0);*/


	DDRF |= (1 << PF0);
	bit_set(PORTF, BIT(0));

	DDRG |= (1 << PG4);
	PORTG |= (1 << PG4);

	_delay_us(1);

	char my_str[10];

	WireInfo info;
	WireInfo wires[7];

	char buffer[2];
	char str[10];
	eepromRead(buffer,address,2);
	itoa(buffer[0],str,10);
	GLCD_SetCursorAddress(80);
	GLCD_WriteText(str);

	FIL f_map;

	if(f_open(&f_map,"/MAP.CHK",FA_READ | FA_WRITE | FA_OPEN_ALWAYS) != FR_OK)
	{
		GLCD_SetCursorAddress(0);
		GLCD_WriteText("Error Opening");
		return 2;
	}

	uint8_t test_vector[18];
	int i,j; uint8_t position;
	unsigned int bytes_written;

	setFirstBitOnBoard(ControllerBoard);
	for(i=0;i<72;i++)
	{
		GLCD_SetCursorAddress(120);
		recieveTestVectorFromConnectedBoards(test_vector);
		for(j=0;j<18;j++)
		{
			itoa(test_vector[j],str,2);
			GLCD_WriteText(str);
			GLCD_WriteText("-");
		}

		f_write(&f_map,test_vector,18,&bytes_written);
		if(bytes_written == 0)
		{
			GLCD_ClearGraphic();
			GLCD_ClearText();
			GLCD_SetCursorAddress(0x00);
			GLCD_WriteText("ERROR writing to MMC/SD Card. Contact supervisor.");
			return 1;
		}
		//_delay_ms(100);
		shiftVectorOnBoard(ControllerBoard);
	}

	setFirstBitOnBoard(DaughterBoard_One);
	for(i=0;i<72;i++)
	{
		GLCD_SetCursorAddress(120);
		recieveTestVectorFromConnectedBoards(test_vector);
		for(j=0;j<18;j++)
		{
			itoa(test_vector[j],str,2);
			GLCD_WriteText(str);
			GLCD_WriteText("-");
		}

		f_write(&f_map,test_vector,18,&bytes_written);
		if(bytes_written == 0)
		{
			GLCD_ClearGraphic();
			GLCD_ClearText();
			GLCD_SetCursorAddress(0x00);
			GLCD_WriteText("ERROR writing to MMC/SD Card. Contact supervisor.");
			return 1;
		}
		//_delay_ms(100);
		shiftVectorOnBoard(DaughterBoard_One);
	}

	GLCD_SetCursorAddress(160);
	GLCD_WriteText("DONE");
	f_close(&f_map);

	return 0;
}

uint8_t learnHarness(void)
{
	_delay_ms(1000);
	unsigned int bytes_written;
	uint16_t total_bytes_written = 0;
	uint8_t recieved_vector[TESTPOINTS/8];

	FIL f_map;

	if(f_open(&f_map,"/MAP.CHK",FA_READ | FA_WRITE | FA_OPEN_ALWAYS) != FR_OK)
	{
		GLCD_SetCursorAddress(0);
		GLCD_WriteText("Error Opening");
		return 2;
	}

	setFirstBit();

	for(int i=0;i<TESTPOINTS;i++)
	{
		recieveTestVector(recieved_vector);
		f_write(&f_map,recieved_vector,TESTPOINTS/8,&bytes_written);
		if(bytes_written == 0)
		{
			GLCD_ClearGraphic();
			GLCD_ClearText();
			GLCD_SetCursorAddress(0x00);
			GLCD_WriteText("ERROR writing to MMC/SD Card. Contact supervisor.");
			return 1;
		}
		total_bytes_written = total_bytes_written + bytes_written;
		GLCD_SetCursorAddress(0x00);

		shiftLeft();
	}
	f_close(&f_map);

	return 0;
}
