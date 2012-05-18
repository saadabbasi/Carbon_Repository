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

#define TPOINTS 		360
#define TPOINTS_BYTES	45

typedef enum
{
	CH_OK = 0,
	CH_NO_SD_CARD_PRESENT,
	INVALID_BOARD_SEQUENCE,
} CH_RESULT;

CH_RESULT programHarness(void);
uint8_t initalizeDriverCPLDs(void);
void recieveTestVectorFromConnectedBoards(uint8_t test_vector[]);

#endif /* CHECKERAPI_H_ */
