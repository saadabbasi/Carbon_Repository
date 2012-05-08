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

int8_t returnNumberOfConnectedBoards(void);
void recieveTestVectorFromConnectedBoards(uint8_t test_vector[]);

#endif /* CHECKERAPI_H_ */
