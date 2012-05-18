#include "25LC512.h"

void writeEnable(void)
{
	uint8_t WREN = 0x06;

	selectDevice(EEPROM);
	writeByte(WREN);
	selectDevice(DESELECT);
}

void eepromSendAddress(uint16_t address)
{
	writeByte(address >> 8);
	writeByte(address);
}

void eepromWrite(uint8_t byte, uint16_t address)
{
	const uint8_t WRITE = 0x02;

		writeEnable();

		_delay_us(1);

		selectDevice(EEPROM);
		writeByte(WRITE);
		eepromSendAddress(address);

		for (int i = 0; i < 128; i++) {
			writeByte(byte);
		}
		selectDevice(DESELECT);
		_delay_ms(10);
}

void eepromWritePage(uint8_t page[], uint16_t address)
{
	const uint8_t WRITE = 0x02;

		writeEnable();

		selectDevice(EEPROM);
		writeByte(WRITE);

		eepromSendAddress(address);

		for (int i = 0; i < 128; i++) {
			writeByte(page[i]);
		}
		selectDevice(DESELECT);
		_delay_ms(10);
}

void eepromRead(char buffer[], uint16_t address, uint8_t bytes_to_read)
{
	const uint8_t READ = 0x03;

	char str[10];
	for(int i = 0;i<10;i++)
	{
		str[i] = 0x00;
	}

	selectDevice(EEPROM);
	writeByte(READ);
	eepromSendAddress(address);

	for(int i=0;i<bytes_to_read;i++)
	{
		buffer[i] = readByte();
	}
	
	GLCD_WriteText(str);
	selectDevice(DESELECT);
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
