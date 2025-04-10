/*******************************************************************************
*                           TINY0_macro.h
* File: TINY0_macro.h
* Target MCU  : TINY V0
* Programmer :  Nikola Kirov
*******************************************************************************/
#ifndef PRR
#ifdef  PRR0
	#define PRR									PRR0
#endif
#endif

#define RESET()									sreset()

#ifdef __GNUC__
	#define __PROGMEM							PROGMEM
	#define READ_BYTE(const_arr, index)			__LPM(const_arr + index)
	#define READ_WORD(const_arr, index)			__LPM_word(const_arr + index)
	#define READ_DWORD(const_arr, index)		__LPM_dword(const_arr + index)
	#define READ_FLOAT(const_arr, index)		__LPM_float(const_arr + index)

	#define ENTER_CRITICAL()					\
	{   										\
		u8 SaveSREG = SREG;						\
		cli();

	#define EXIT_CRITICAL()						\
		SREG = SaveSREG;						\
	}

	#define WDR()								__asm__ __volatile__ ("wdr")
	#define WD_RESET()							__asm__ __volatile__ ("wdr")
	#define wdt_disable()

	#define irqen()								sei()
	#define irqdis()							cli()   

#else //====== IAR =======
	#define __PROGMEM							__flash
	#define READ_BYTE(const_arr, index)			__load_program_memory((const unsigned char __flash *)const_arr + index)
	#define READ_WORD(const_arr, index)			(*(u16 __flash *)(a))
	#define READ_DWORD(const_arr, index)		(*(u32 __flash *)(a))
	#define READ_FLOAT(const_arr, index)		(*(float __flash *)(a))

	#define ENTER_CRITICAL(){   							\
		unsigned char SaveSREG = __save_interrupt();		\
		__disable_interrupt();                                  
			
	#define EXIT_CRITICAL()	 __restore_interrupt(SaveSREG);  }
			
	#define WD_RESET()                      	__watchdog_reset()
	#define WDR()								__watchdog_reset()	
	#define wdt_disable()
		
	#define irqen()								__enable_interrupt()
	#define irqdis()							__disable_interrupt()
#endif
	

	/* =================  Platform Independent  ========================*/	
    /* IO MACROS */
#define PG(port)							PG_EXP(port)                    // Get pin states
#define PD(port,value)						PORT_DIR_EXP(port,value)        // TRUE = output
#define PDG(port)							PORT_DIR_GET_EXP(port,value)    // TRUE = output

    /* IO by mask */
#define PMDO(port,mask)						PMDO_EXP(port,mask)             // Set direction to output
#define PMDI(port,mask)						PMDI_EXP(port,mask)             // Set direction to input
#define PMS(port,mask)						PMS_EXP(port,mask)              // Set output
#define PMC(port,mask)						PMC_EXP(port,mask)              // Clear output

    /* Platform Independent for bits*/
#define PBS(port,bit)						PBS_EXP(port,bit)               // PORT bit set
#define PBC(port,bit)						PBC_EXP(port,bit)               // Port bit clear
#define PBT(port,bit)						PBT_EXP(port,bit)               // Port bit toggle
#define PBG(port,bit)						PBG_EXP(port,bit)				// Port bit get
#define PBDO(port,bit)						PBDO_EXP(port,bit)				// Port bit direction out
#define PBDI(port,bit)						PBDI_EXPR(port,bit)				// Port bit direction input
#define PBPUE(port,bit)						PBPUE_EXPR(port,bit)			// Port bit pull up enable

#define PORT(a,value)						PORT_EXP(a,value)



//------------------------------------------------------------------------------
//			Macro expressions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#define PBDO_EXP(port,bit)  				PORT ## port.DIRSET = (1<<bit)
#define PBDI_EXPR(port,bit) 				PORT ## port.DIRCLR = (1<<bit)

#define PBS_EXP(port,bit)					PORT ## port.OUTSET = (1<<bit)
#define PBC_EXP(port,bit)					PORT ## port.OUTCLR = (1<<bit)
#define PBT_EXP(port,bit)					PORT ## port.OUTTGL = (1<<bit)
		
#define PBPUE_EXPR(port,bit)				(PORT ## port.PIN ## bit ## CTRL |= PORT_PULLUPEN_bm)

#define PBG_EXP(port,bit)					(PORT ## port.IN & (1<<bit))
	
#define PMDO_EXP(port,mask)             	PORT ## port.DIRSET = mask
#define PMDI_EXP(port,mask)             	PORT ## port.DIRCLR = mask

#define PMS_EXP(port,mask)              	PORT ## port.OUTSET = mask
#define PMC_EXP(port,mask)              	PORT ## port.OUTCLR = mask

#define PORT_EXP(port,value)               	PORT ## port.OUT = value
#define PG_EXP(port)                    	PORT ## port.IN
