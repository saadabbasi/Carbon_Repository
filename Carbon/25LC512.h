#include <stdint.h>
#include "SPIController.h"

#define EEPROM_PAGESIZE			128
#define	DESELECT				0x00

extern void sendByte(uint8_t);
extern uint8_t readByte(void);

void writeEnable(void);
void eepromSendAddress(uint16_t address);
void eepromWrite(uint8_t byte, uint16_t address);
void eepromWritePage(uint8_t page[], uint16_t address);
void eepromRead(char buffer[], uint16_t address, uint8_t BytesToRead);
void eepromChipErase(void);
void eepromPageErase(void);
uint8_t eepromReadStatusRegister(void); // use to check busy flag.
