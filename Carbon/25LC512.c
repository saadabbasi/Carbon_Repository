#include "25LC512.h"

uint8_t eepromWriteInProgress(void)
{
	uint8_t status_reg = eepromReadStatusRegister();
	if(status_reg & WIP_MASK)
	{
		return 1;
	}
	return 0;
}

void writeEnable(void)
{
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

uint8_t eepromWritePage(uint8_t page[], uint16_t address)
{
	uint8_t status_reg = eepromReadStatusRegister();
	if (status_reg && 0x0C)
	{
		return 1;
	}
	writeEnable();

	selectDevice(EEPROM);
	writeByte(WRITE);

	eepromSendAddress(address);

	for (int i = 0; i < 128; i++) {
		writeByte(page[i]);
	}
	selectDevice(DESELECT);
	_delay_ms(10);
	return 0;
}

void eepromRead(char buffer[], uint16_t address, uint8_t bytes_to_read)
{
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
	selectDevice(DESELECT);
}
void eepromChipErase(void)
{
	writeEnable();
	selectDevice(EEPROM);
	writeByte(CHIPERASE);
	selectDevice(DESELECT);
	_delay_ms(100);
}
void eepromPageErase(void)
{

}

void eepromRemoveProtectionFromAllSectors(void)
{
	uint8_t status_register;
	status_register = 0x00;
	eepromWriteStatusRegister(status_register);
}


void eepromWriteProtectAllSectors(void)
{
	uint8_t status_register;
	status_register = 0x0C;
	eepromWriteStatusRegister(status_register);
}

void eepromWriteStatusRegister(uint8_t status_register)
{
	writeEnable();
	selectDevice(EEPROM);
	writeByte(WRSR);
	writeByte(status_register);
	selectDevice(DESELECT);
}

uint8_t eepromReadStatusRegister(void)
{
	selectDevice(EEPROM);
	writeByte(RDSR);
	uint8_t status = readByte();
	selectDevice(DESELECT);
	return status;
}
