/*
 *  TINY0_EEprom.h
 *  Author: Nikola Kirov
 */
#ifndef _TINY0_EE_H_
#define _TINY0_EE_H_
#ifdef __cplusplus
	extern "C" {
#endif

/*
	Read a byte from eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
Return value:
	The read byte
*/
u8 iee_read_byte(u16 eeprom_adr);

/*
	Read a 16 bit value from eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	out: Output
*/
void iee_read_word(u16 eeprom_adr, void *out);

/*
	Read a 32 bit value from eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	out: Output
*/
void iee_read_dword(u16 eeprom_adr, void *out);

/*
	Write a 16 bit value to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	data: pointer to 16 bit data
*/
void iee_write_word(u16 eeprom_adr, void *data);

/*
	Write a 32 bit value to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	data: pointer to 32 bit data
*/
void iee_write_dword(u16 eeprom_adr, void *data);

/*
	Write a byte to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to write to
	data: The byte to write
 */
void iee_write_byte(u16 eeprom_adr, u8 data);

/*
	Read a block from eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	buff: Buffer to place read data into
 */
void iee_read_buff(u16 eeprom_adr, void *buff, u8 size);

/*
	Write a block to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to write to
	buff: The buffer to write
	size: Size of the buffer
 */	  
void iee_write_buff(u16 eeprom_adr, const void *buff, u8 size);
	
#ifdef __cplusplus
}
#endif
#endif /* _TINY0_EE_H_ */
