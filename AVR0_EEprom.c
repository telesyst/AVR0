/*
 *	AVR0_EEprom.c
 *  Author: Nikola Kirov
 */
#include "AVR0.h"

#ifdef ASTART_PROJECT
	#include "ccp.h"
#endif

#ifdef MCC_PROJECT
	#include "../mcc_generated_files/system/ccp.h"
#endif

static void _iee_wait_for_write_completion(void)
{
	while (NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm)
		;
}

/*
	Read a byte from eeprom
Arguments:
	eeprom_adr The byte-address in eeprom to read from
Return value:
	The read byte
*/
u8 iee_read_byte(u16 eeprom_adr)
{
	// Read operation will be stalled by hardware if any write is in progress
	return *(u8 *)(EEPROM_START + eeprom_adr);
}

/*
	Read a 16 bit value from eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	out: Output
*/
void iee_read_word(u16 eeprom_adr, void *out)
{
	*(u16*)out = *(u16 *)(EEPROM_START + eeprom_adr);
}

/*
	Read a 32 bit value from eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	out: Output
*/
void iee_read_dword(u16 eeprom_adr, void *out)
{
	*(u32*)out = *(u32 *)(EEPROM_START + eeprom_adr);
}

/*
	Write a 16 bit value to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	data: pointer to 16 bit data
*/
void iee_write_word(u16 eeprom_adr, void *data)
{
	iee_write_buff(eeprom_adr, data, 2);
}

/*
	Write a 32 bit value to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	data: pointer to 32 bit data
*/
void iee_write_dword(u16 eeprom_adr, void *data)
{
	iee_write_buff(eeprom_adr, data, 4);
}

/*
	Write a byte to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to write to
	data: The byte to write
 */
void iee_write_byte(u16 eeprom_adr, u8 data)
{
	_iee_wait_for_write_completion();
	/* Clear page buffer */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEBUFCLR_gc);
	/* Write byte to page buffer */
	*(u8 *)(EEPROM_START + eeprom_adr) = data;
	/* Erase byte and program it with desired value */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEERASEWRITE_gc);
}

/*
	Read a block from eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to read from
	buff: Buffer to place read data into
 */
void iee_read_buff(u16 eeprom_adr, void *buff, u8 size)
{
	u8 *data = (u8*)buff;
	// Read operation will be stalled by hardware if any write is in progress
	memcpy(data, (u8 *)(EEPROM_START + eeprom_adr), size);
}

/*
	Write a block to eeprom
Arguments:
	eeprom_adr: The byte-address in eeprom to write to
	buff: The buffer to write
	size: Size of the buffer
 */
void iee_write_buff(u16 eeprom_adr, const void *buff, u8 size)
{
	const u8 * data = (const u8 *)buff;
	u8 *write = (u8 *)(EEPROM_START + eeprom_adr);
	
	_iee_wait_for_write_completion();

	/* Clear page buffer */
	ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEBUFCLR_gc);

	do 
	{
		/* Write byte to page buffer */
		*write++ = *data++;
		size--;
		// If we have filled an entire page or written last byte to a partially filled page
		if ((((uintptr_t)write % EEPROM_PAGE_SIZE) == 0) || (size == 0)) 
		{
			_iee_wait_for_write_completion();
			/* Write page */
			ccp_write_spm((void *)&NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEERASEWRITE_gc);
		}
	} while (size != 0);
}






