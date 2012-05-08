/*
 * harnessio.h
 *
 *  Created on: Apr 2, 2012
 *      Author: saad
 */

#include "constants.h"
#include "ff.h"
#include "25LC512.h"
#include <util/delay.h>

typedef struct
{
	char colour[COLOUR_WIDTH];
	char gauge[GAUGE_WIDTH];
	char locationA[LOCATION_WIDTH];
	char locationB[LOCATION_WIDTH];
} WireInfo;

#ifndef HARNESSIO_H_
#define HARNESSIO_H_

#define CMD_SHIFTLEFT 0xAA
#define CMD_ALLZEROES 0x0F
#define CMD_SETFIRSTBIT 0x01
#define CMD_GETSETBITPOSITION 0xF0

void recieveTestVector(uint8_t testVector[]);
void chipSelect(uint8_t chipID);
void shiftLeft(void);
void setFirstBit(void);
uint8_t getSetBitPosition(void);
extern uint8_t readByte(void);
extern void sendByte(uint8_t);

void read_ckt_data(FIL *f_ptr, int ckt_num, unsigned char ckt_data[]);
void fetchWireInfo(FIL *f_ptr, int wire_num, char wire_info[], int parameter, int width);
void getWireInfo(FIL *f_ptr, int wire_num, WireInfo *wire);
void returnWireInfo(FIL *f_ptr, int16_t wire_position[], WireInfo wire[]);
void getHarnessName(FIL *f_ptr, char harness_name[]);
void readWireParameters(uint16_t wire_num, WireInfo *wireInfo);
void readWireParametersFromIndices(uint16_t wire_positions[], WireInfo wireInfo[]);
uint8_t copyHarnessCircuitData(void);
uint8_t verifyHarnessCircuitData(void);
#endif /* HARNESSIO_H_ */
