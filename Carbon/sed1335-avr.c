#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "sed1335.h"
#include "fonts.h"

#define SED1335_DATA_PORT		PORTC
#define SED1335_DATA_DIR		DDRC
#define SED1335_DATA_PIN		PINC

#define SED1335_CONTROL_PORT	PORTD
#define SED1335_CONTROL_DIR		DDRD
#define	SED1335_CONTROL_PIN		PIND

#define SED1335_A0				(1 << PD0)
#define SED1335_WR				(1 << PD1)
#define SED1335_RD				(1 << PD2)
#define SED1335_CS				(1 << PD3)
#define SED1335_RES				(1 << PD4)

void GLCD_InitPorts(void)
{
	SED1335_DATA_DIR = 0xFF;
	SED1335_CONTROL_PORT |= (SED1335_A0 | SED1335_WR | SED1335_RD | SED1335_CS | SED1335_RES); 
	SED1335_CONTROL_DIR |= (SED1335_A0 | SED1335_WR | SED1335_RD | SED1335_CS | SED1335_RES); 
}

void GLCD_WriteData(unsigned char dataToWrite)
{
	SED1335_DATA_PORT = dataToWrite;
	SED1335_DATA_DIR = 0xFF;
	SED1335_CONTROL_PORT &= ~(SED1335_CS | SED1335_A0 | SED1335_WR);
	asm("nop");
	SED1335_CONTROL_PORT |= (SED1335_CS | SED1335_A0 | SED1335_WR);
}

void GLCD_WriteCommand(unsigned char commandToWrite)
{
	SED1335_DATA_PORT = commandToWrite;
	SED1335_DATA_DIR = 0xFF;
	SED1335_CONTROL_PORT &= ~(SED1335_CS |SED1335_WR);
	asm("nop");
	SED1335_CONTROL_PORT |= (SED1335_CS | SED1335_WR);
}

unsigned char GLCD_ReadData(void)
{
	unsigned char tmp;
	SED1335_CONTROL_PORT &= ~(SED1335_CS | SED1335_RD);
	asm("nop");
	SED1335_DATA_DIR = 0x00;
	asm("nop");
	tmp =  SED1335_DATA_PIN;
	SED1335_CONTROL_PORT |= (SED1335_CS | SED1335_RD);
	return tmp;
}

char GLCD_ReadByteFromROMMemory(char * ptr)
{
	return pgm_read_byte(ptr);
}

void drawAChar(uint16_t x, int y, int width, int height, unsigned int offset, const unsigned char *fontBitmap)
{
	int pos = offset;
	unsigned char data;
	for(int i = 0; i < height-1 ; i++)
	{
		GLCD_GraphicGoTo(x, y+i);
		GLCD_WriteCommand(SED1335_MWRITE);
		for(int j = 0; j < width/8 ; j++)
		{
			data = pgm_read_byte(&fontBitmap[pos]);
			GLCD_WriteData(data);
			pos++;	
		}
	}
}

void drawSmallText(const char text[], int length, int x, int y)
{
	int index = 0;
	int width = 16;
	int height = 18;

	for(int i=0;i<length;i++)
	{
		if(text[i] == ' ')
		{
			index = 43;
			width = 16;
		}
		else if(text[i] == '-')
		{
			index = 44;
			width = 16;
		}
		else if(text[i] == '/')
		{
			index = 45;
			width = 16;
		}
		else if(text[i] == '.')
		{
			index = 46;
			width = 8;
		}
		else if(text[i] == ':')
		{
			index = text[i] - '0';
			width = 8;
		}
		else if(text[i] >= '0' && text[i]<='Z')
		{
			index = text[i] - '0';
			width = 16;
		}
		else
		{
			index = 43;
			width = 16;
		}

		FONT_CHAR_INFO smallType = proFontWindows20ptDescriptors[index];
		drawAChar(x,y,width,height,smallType.offset,proFontWindows20ptBitmaps);
		x = x + width;
	}
}


void drawLargeText(const char text[], int length, uint16_t x, int y)
{
	uint8_t index = 0;
	uint8_t width = 24;
	uint8_t height = 40;

	for(uint8_t i=0;i<length;i++)
	{
		if(text[i] == ' ')
		{
			width = 24;
		}
		else if(text[i] >= '!' && text[i]<=']')
		{
			//index = (text[i] - '0') + 1;
			//GLCD_SetCursorAddress(0);

			index = text[i] - proFontWindows40ptFontInfo[1];
			//index = index + 1;
		}

		if(text[i] >= '!' && text[i]<=']')
		{
			FONT_CHAR_INFO largeType = proFontWindows36ptDescriptors[index];
			width = largeType.widthBits;
			drawAChar(x,y,width,height,largeType.offset,proFontWindows40ptBitmaps);
		}
		x = x + width;
	}
}

void drawText(const char text[], int length, uint16_t x, int y, char FONT_SIZE)
{
	if(FONT_SIZE == 20)
		drawSmallText(text,length,x,y);
	else if(FONT_SIZE == 48)
		drawLargeText(text,length,x,y);
}
