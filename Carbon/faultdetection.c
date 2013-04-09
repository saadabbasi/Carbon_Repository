/*
 * faultdetection.c
 *
 *  Created on: May 23, 2012
 *      Author: saad
 */

#include "faultdetection.h"

uint16_t detectShortCircuitsAndReturnCount(uint8_t expected_vector[], uint8_t faults[], uint8_t short_circuits[], uint8_t TBYTES)
{
	uint16_t short_circuit_count = 0;

	for(int i = 0; i<TBYTES; i++)
	{
		short_circuits[i] = faults[i] & expected_vector[i];
		short_circuit_count = short_circuit_count + countBitsSet(short_circuits[i]);
	}

	return short_circuit_count;
}

uint16_t detectOpenCircuitsAndReturnCount(uint8_t expected_vector[], uint8_t vector_under_test[], uint8_t open_circuits[], uint8_t TBYTES)
{
	uint16_t open_circuit_count;

	open_circuit_count = 0;

	for(int i = 0; i < TBYTES; i++)
	{
		open_circuits[i] = expected_vector[i] | vector_under_test[i];
		open_circuits[i] = open_circuits[i] ^ expected_vector[i];
		open_circuit_count = open_circuit_count + countBitsSet(open_circuits[i]);
	}

	return open_circuit_count;
}

uint16_t detectFaultsAndReturnCount(uint8_t expected_vector[], uint8_t vector_under_test[], uint8_t faults[], uint8_t TBYTES)
{
	uint16_t fault_count = 0;

	for(int i = 0; i < TBYTES; i++)
	{
		faults[i] = expected_vector[i] ^ vector_under_test[i];
		fault_count = fault_count + countBitsSet(faults[i]);
	}

	return fault_count;
}

int countBitsSet(uint8_t byte)
{
	unsigned char v; // count the number of bits set in v
	unsigned int c; // c accumulates the total bits set in v
	v = byte;
	for (c = 0; v; v >>= 1)
	{
		c += v & 1;
	}
	return c;
}

uint16_t returnFaultyWirePositions(uint8_t seqToCheck[], int16_t bit_positions[], uint16_t TBYTES)
{
	int i;
	uint16_t fault_count = 0;
	uint16_t total_test_points = TBYTES * 8;

	for(i=0;i<TPOINTS;i++)
	{
		bit_positions[i] = -1;
	}
	int pos = 0;
	for(int wire=0; wire < total_test_points; wire++)
	{
		int index = wire / 8;
		uint8_t mask = 1 << (wire & 0x7);
		if(seqToCheck[index] & mask)
		{
			int offset = returnPositionOfOne(mask);
			bit_positions[pos] = total_test_points-(offset + ((TBYTES-1)-index)*8);
			pos++;
			fault_count++;
		}
	}

	bubbleSort(bit_positions);

	return fault_count;
}

void bubbleSort(int16_t arrayToSort[])
{
	int i, j, temp;

	for(i=TPOINTS; i > 0; i--)
	{
		for(j = 1; j <= i; j++)
		{
			if(arrayToSort[j-1] > arrayToSort[j])
			{
				if(arrayToSort[j] != -1)
				{
					temp = arrayToSort[j-1];
					arrayToSort[j-1] = arrayToSort[j];
					arrayToSort[j] = temp;
				}

			}
		}
	}

}

uint8_t returnPositionOfOne(uint8_t byteToCheck)
{
	switch(byteToCheck)
	{
	case 128:
		return 7;
		break;
	case 64:
		return 6;
		break;
	case 32:
		return 5;
		break;
	case 16:
		return 4;
		break;
	case 8:
		return 3;
		break;
	case 4:
		return 2;
		break;
	case 2:
		return 1;
		break;
	case 1:
		return 0;
		break;
	}
	return 0;
}
