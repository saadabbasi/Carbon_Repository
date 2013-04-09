/*
 * faultdetection.h
 *
 *  Created on: May 23, 2012
 *      Author: saad
 */

#ifndef FAULTDETECTION_H_
#define FAULTDETECTION_H_

#include <avr/io.h>
#include <stdint.h>
#include "CPLD_API.h"
#include "25LC512.h"
#include "CheckerAPI.h"

uint16_t detectShortCircuitsAndReturnCount(uint8_t expected_vector[], uint8_t faults[], uint8_t short_circuits[], uint8_t TBYTES);
uint16_t detectOpenCircuitsAndReturnCount(uint8_t expected_vector[], uint8_t vector_under_test[], uint8_t open_circuits[], uint8_t TBYTES);
uint16_t detectFaultsAndReturnCount(uint8_t expected_vector[], uint8_t vector_under_test[], uint8_t faults[], uint8_t TBYTES);
int countBitsSet(uint8_t byte);
uint16_t returnFaultyWirePositions(uint8_t seqToCheck[], int16_t bit_positions[], uint16_t TBYTES);
uint8_t returnPositionOfOne(uint8_t byteToCheck);
void bubbleSort(int16_t arrayToSort[]);

#endif /* FAULTDETECTION_H_ */
