#ifndef EEPROM_H_
#define EEPROM_H_

UC EEPROM_ONCHIP_read(UI address);
void EEPROM_ONCHIP_write(UI address, UC data);
void EEPROM_write(uint16_t address, uint8_t data, uint8_t CHIP_SELECT);
uint8_t EEPROM_read(uint16_t address, uint8_t CHIP_SELECT_WRITE, uint8_t CHIP_SELECT_READ);

#endif /* EEPROM_H_ */