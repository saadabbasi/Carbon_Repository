#include <stdint.h>
#include "SPIController.h"

#define EEPROM_PAGESIZE			128
#define EEPROM_SIZE				65536

#define WREN  		6
#define WRDI  		4
#define RDSR  		5
#define WRSR  		1
#define READ  		3
#define WRITE 		2
#define CHIPERASE	0xC7

#define WIP_MASK	0x01

uint8_t eepromWriteInProgress(void);
void writeEnable(void);
void eepromSendAddress(uint16_t address);
void eepromWrite(uint8_t byte, uint16_t address);
uint8_t eepromWritePage(uint8_t page[], uint16_t address);
void eepromRead(char buffer[], uint16_t address, uint8_t BytesToRead);
void eepromChipErase(void);
void eepromPageErase(void);
void eepromRemoveProtectionFromAllSectors(void);
void eepromWriteProtectAllSectors(void);
void eepromWriteStatusRegister(uint8_t status_register);
uint8_t eepromReadStatusRegister(void); // use to check busy flag.
