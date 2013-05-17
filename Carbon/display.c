/*
 * display.c
 *
 *  Created on: May 25, 2012
 *      Author: saad
 */

#include "display.h"
#include <math.h>
#include "LockIcon.h"

void displayMessage(char message[])
{
	GLCD_ClearGraphic();
	GLCD_ClearText();
	uint8_t length = strlen(message);
	drawText(message,length,320/2-(length*16/2),240/2-9,20);
}

void displayOKScreen(void)
{
	displayMessage("HARNESS OK");
}

void displayFaults(WireInfo faulty_wire[], const char* fault_type, uint16_t no_of_faults)
{
	char locations[20] = {'\0'};
	uint8_t font_size;
	char colon[2] = ":";
	char colour_width[COLOUR_WIDTH + GAUGE_WIDTH];
	GLCD_ClearGraphic();

	if(faulty_wire[0].locationB[0] == '-')
	{
		faulty_wire[0].locationB[1] = '-';
		faulty_wire[0].locationB[2] = 0x00;
	}
	int lenA = strlen(faulty_wire[0].locationA);
	int lenB = strlen(faulty_wire[0].locationB);
	uint8_t totalLength = lenA + lenB;

	char str[5] = {'\0'};
	font_size = 48;
	if(totalLength < 7)
	{
		strcpy(str,"    ");
	}
	else if(totalLength >= 7 && totalLength < 11)
	{
		strcpy(str,"  ");
	}
	else if(totalLength >= 11 && totalLength < 13)
	{
		strcpy(str," ");
	}
	else
	{
		strcpy(str,"   ");
		font_size = 20;
	}

	strcpy(locations, faulty_wire[0].locationA);
	strcat(locations, str);
	strcat(locations, faulty_wire[0].locationB);

	int lengthOfLocations = strlen(locations);
	uint16_t textWidth = textWidthOfString(locations,font_size) - 4 - 1;
	uint16_t textStart = ((SED1335_SCR_WIDTH )-textWidth)/2;
	drawText(locations,lengthOfLocations,textStart,25,font_size);

	drawText(fault_type,strlen(fault_type),(SED1335_SCR_WIDTH+1)/2 - strlen(fault_type)*16/2,98,20);

	for(int i=0;i<SED1335_SCR_WIDTH/2-(8*17/2);i++)
	{
		GLCD_SetPixel(i,140,0xFF);
	}
	for(int i=SED1335_SCR_WIDTH/2 + (8*17/2) + 5;i<SED1335_SCR_WIDTH;i++)
	{
		GLCD_SetPixel(i,140,0xFF);
	}
	GLCD_SetCursorAddress(40*17+40/2 - 17/2);
	GLCD_WriteText("Additional Faults");

	uint8_t h_offset = 0;
	uint8_t w_offset = 0;
	const uint8_t line_height = 30;
	for(int k=1;k<no_of_faults;k++)
	{
		if(k%2 == 0)
		{
			w_offset = 1;
		}
		else
		{
			w_offset = 0;
		}
		uint8_t width = textWidthOfString(faulty_wire[k].locationA,20);
		drawText(faulty_wire[k].locationA,strlen(faulty_wire[k].locationA),25+w_offset*15*10,150 + line_height*h_offset,20);
		drawText(colon,strlen(colon),25+w_offset*15*10+width,150 + line_height*h_offset,20);
		width = width + 7;
		drawText(faulty_wire[k].locationB,strlen(faulty_wire[k].locationB),25+w_offset*15*10+width,150 + line_height*h_offset,20);
		h_offset = k/2;
	}

	strncpy(colour_width,faulty_wire[0].colour,COLOUR_WIDTH);
	strncat(colour_width," ",1);

	if(faulty_wire[0].gauge[0] != '#') // if gauge is '#', then the faulty location is considered to be a missing clip.
	{
		strncat(colour_width,faulty_wire[0].gauge,GAUGE_WIDTH);
	}
	else
	{
		strncat(colour_width,"--",GAUGE_WIDTH);
	}

	drawText(colour_width,strlen(colour_width),(SED1335_SCR_WIDTH+1)/2 - strlen(colour_width)*16/2,75,20);
}

uint16_t textWidthOfString(char str[], char FONT_SIZE)
{
	if(FONT_SIZE == 20)
	{
		return strlen(str)*16;
	}
	else
		return strlen(str)*24;

}
