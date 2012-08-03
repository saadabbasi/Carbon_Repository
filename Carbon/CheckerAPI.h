/*
 * CheckerAPI.h
 *
 *  Created on: May 8, 2012
 *      Author: saad
 */

#ifndef CHECKERAPI_H_
#define CHECKERAPI_H_

#include <avr/io.h>
#include <stdint.h>
#include "CPLD_API.h"
#include "uart.h"
#include "xitoa.h"
#include "ff.h"
#include "diskio.h"
#include "25LC512.h"
#include "faultdetection.h"
#include "constants.h"

#define TPOINTS 			360
#define TPOINTS_BYTES		45
#define	TPOINTS_PER_BOARD	72
#define BYTES_PER_BOARD		9
#define CKT_HEADER_ADDR		32640
#define LOC_HEADER_ADDR		65408
#define LOC_DATA_ADDR		0x8000
#define CKT_HEADER_LENGTH	10
#define LOC_HEADER_LENGTH	25
#define MAX_DAUGHTER_BOARDS	5

typedef enum
{
	CH_OK = 0,
	CH_SHORT_CIRCUIT,
	CH_MISSING_WIRE,
	CH_WRONG_SLOT,
	CH_NO_SD_CARD_PRESENT,
	CH_INVALID_BOARD_SEQUENCE,
	CH_INVALID_FIL,
	CH_INVALID_HEADER_CKT,
	CH_INVALID_HEADER_LOC,
	CH_VERIFICATION_FAILED,
	CH_NOT_ERASED
} CH_RESULT;

typedef struct
{
	char colour[COLOUR_WIDTH];
	char gauge[GAUGE_WIDTH];
	char locationA[LOCATION_WIDTH];
	char locationB[LOCATION_WIDTH];
} WireInfo;

uint8_t detectedCPLDs(void);
CH_RESULT isEEPROMErased(void);
uint8_t returnStoredBoardSequence(void);
void copyEEPROMToFile(void);
void readWireParametersFromIndices(int16_t wire_positions[], WireInfo wireInfo[]);
void readWireParameters(uint16_t wire_num, WireInfo *wireInfo);
CH_RESULT checkBoardSequence(uint8_t connectedBoards);
CH_RESULT findFaultsAndReturnFaultyWireInfos(uint8_t board_count, WireInfo faulty_wires[], uint16_t *count);
CH_RESULT getCKTInfo(uint16_t ckt, uint16_t vector_size, uint8_t cktInfo[]);
CH_RESULT readCKTHeader(FIL *file);
CH_RESULT copyCKTFileToEEPROM(void);
CH_RESULT copyLOCFileToEEPROM(void);
CH_RESULT verifyCKTFile(void);
CH_RESULT verifyLOCFile(void);
CH_RESULT programHarness(uint8_t board_count, uint8_t connectedBoards);
CH_RESULT initalizeDriverCPLDs(uint8_t *count, uint8_t *inited_boards);
void recieveTestVectorFromConnectedBoards(uint8_t test_vector[], uint8_t board_count);

#endif /* CHECKERAPI_H_ */