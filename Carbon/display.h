/*
 * display.h
 *
 *  Created on: May 25, 2012
 *      Author: saad
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "CheckerAPI.h"
#include "sed1335.h"
#include "fonts.h"

#define MAKECURSORINVISBLE	GLCD_SetCursorAddress(5000)

void displayMessage(char message[]);
void displayOKScreen(void);
void displayFaults(WireInfo faulty_wire[], const char *fault_type, uint16_t no_of_faults);
uint16_t textWidthOfString(char str[], char FONT_SIZE);

#endif /* DISPLAY_H_ */
