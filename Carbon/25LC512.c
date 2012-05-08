#include "25LC512.h"

void eepromSendAddress(uint16_t address)
{
	sendByte(address >> 8);
	sendByte(address);
}

//void eepromWrite(uint8_t byte, uint16_t address)
//{

//}
//void eepromWrite(uint8_t page[], uint16_t address)
//{

//}/
void eepromRead(char buffer[], uint16_t address, uint8_t bytes_to_read)
{
	const uint8_t READ = 0x03;

	char str[10];
	for(int i = 0;i<10;i++)
	{
		str[i] = 0x00;
	}

	chipSelect(EEPROM);
	sendByte(READ);
	eepromSendAddress(address);

	for(int i=0;i<bytes_to_read;i++)
	{
		buffer[i] = readByte();
	}
	
	GLCD_WriteText(str);
	chipSelect(DESELECT);
}
void eepromChipErase(void)
{

}
void eepromPageErase(void)
{

}
uint8_t eepromReadStatusRegister(void)
{

}
