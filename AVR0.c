/*
 *	TINY0.c
 *  TINY core v0
 *  Author: Nikola Kirov
 */ 

#define ATOMIC_H  // disable inclusion of atomic.h. 
#include "TINY0.h"

#ifdef ASTART_PROJECT
#include "ccp.h"
#include "atmel_start.h"
#endif

#ifdef MCC_PROJECT
#include "../mcc_generated_files/system/system.h"
#endif

#define WDT_TIMEOUT_PERIOD_8MS						1
#define WDT_TIMEOUT_PERIOD_16MS						2		
#define WDT_TIMEOUT_PERIOD_32MS						3
#define WDT_TIMEOUT_PERIOD_64MS						4
#define WDT_TIMEOUT_PERIOD_125MS					5
#define WDT_TIMEOUT_PERIOD_250MS					6
#define WDT_TIMEOUT_PERIOD_500MS					7
#define WDT_TIMEOUT_PERIOD_1S						8
#define WDT_TIMEOUT_PERIOD_2S						9
#define WDT_TIMEOUT_PERIOD_4S						10
#define WDT_TIMEOUT_PERIOD_8S						11

#ifndef MS_TICK_ENABLE
	#error
#endif

#ifndef BASE_TOOLS_MS_DELAY_HOOK
	#error
#endif

#ifndef USE_IO_CFG_FUNCT
	#error
#endif

#ifdef PROGSTR_BUFF_SIZE
char progstr_buff[PROGSTR_BUFF_SIZE];
#endif

#ifdef SDO_DEBUGS_BUFF_SIZE
char SDO_buff[SDO_DEBUGS_BUFF_SIZE];
#endif

#if !defined(BASE_TOOLS_ENABLE_MS_DELAY)  || !defined(BASE_TOOLS_USE_EXT_MSGET)
	#error
#endif

#if BASE_TOOLS_MS_GET_IS_UNSECURED
	#error // BASE_TOOLS_MS_GET_IS_UNSECURED must be FALSE
#endif

#ifndef TCA0_MS_DELAY_ENABLE
	#error
#endif

#if TCA0_MS_DELAY_ENABLE && MS_TICK_ENABLE
	#error // only one can be allowed 
#endif

#if TCA0_MS_DELAY_ENABLE && !BASE_TOOLS_ENABLE_MS_DELAY
	#error // msDelay = delay_ms
#endif

#if TCA0_MS_DELAY_ENABLE && BASE_TOOLS_USDELAY_IMPLEMENT
	#error // TCA0 used in both 
#endif

#ifndef MS_CORRECT
	#error
#endif

#if TCA0_MS_DELAY_ENABLE
	#ifndef TCA0
		#error
	#endif
	
	#if F_CPU == 500000ul
		#if MS_CORRECT != 2
			#error
		#endif
	#elif F_CPU == 1000000ul
		#if MS_CORRECT != 1
			#error
		#endif
	#elif F_CPU == 2000000ul
		#if MS_CORRECT != 2
			#error
		#endif
	#elif (F_CPU == 4000000ul) 
		#if MS_CORRECT != 4
			#error
		#endif
	#elif (F_CPU == 8000000ul) 
		#if MS_CORRECT != 8
			#error
		#endif
	#elif (F_CPU == 16000000ul) 
		#if MS_CORRECT != 16
			#error
		#endif
	#else
		#error "Only 1Mhz/2MHz/4Mhz/8MHz/16MHz F_CPU supported" 
	#endif
#endif
	
#if TCA0_MS_DELAY_ENABLE && defined(msFromStart)	
	unsigned volatile * tca_counter = (unsigned volatile *)&TCA0.SINGLE.CNT;
#endif

__weak void error_rise_led(ut number)
{
	irqdis();
	for (;;)
	{
		led_flash(number);
		delay_ms(1200);
	}
}

void io_cfg(void);
__weak void io_cfg(void)
{
	
}

void wd_reset(void)
{
	__asm__ __volatile__ ("wdr");
}

void sreset(void)
{
	ccp_write_io((void*)&RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
}

// LED functions
void led_on(void)
{
	LED_ON();
}
void led_off(void)
{
	LED_OFF();
}

void led_toogle(void)
{
	LED_OFF();
}


#if BASE_TOOLS_ENABLE_MS_DELAY  && !BASE_TOOLS_USE_EXT_DELAY && TCA0_MS_DELAY_ENABLE

void msDelay(ms_t delay)
{
	ms_t start = TCA0.SINGLE.CNT;
	while (TRUE)
	{
		if ( msIsTimeElapsedEx(start, TCA0.SINGLE.CNT, delay) )
			return;
		BASE_TOOLS_MS_DELAY_HOOK();
	}
}

ut msIsTimeElapsed(ms_t start_time, ms_t time)
{
#if MS_CORRECT > 1	
	time *= MS_CORRECT;
#endif
	return (TCA0.SINGLE.CNT - start_time) >= time;
}

#endif

#if TCA0_MS_DELAY_ENABLE && !BASE_TOOLS_USE_EXT_MSGET
ms_t msGet(void)
{
	return TCA0.SINGLE.CNT;
}
#endif

void TINY0_init( void )
{
#ifdef ASTART_PROJECT	
	atmel_start_init();
#endif

#ifdef MCC_PROJECT
	SYSTEM_Initialize();
#endif
		
#if USE_IO_CFG_FUNCT
	io_cfg();
#endif
	
	LED_CONFIG();

#if WD_ENABLE
	ccp_write_io((void *)&WDT.CTRLA, WD_PERIODD);
#endif

#if TCA0_MS_DELAY_ENABLE
	STATIC_ASSERT(sizeof(ms_t) == 2);
#if F_CPU == 500000ul
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV256_gc /* System Clock / 1024 */
		| 1 << TCA_SINGLE_ENABLE_bp /* Module Enable: enabled */;
#else	//#if F_CPU == 500000ul
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc /* System Clock / 1024 */
	    | 1 << TCA_SINGLE_ENABLE_bp /* Module Enable: enabled */;
#endif // #if F_CPU == 500000ul
#endif // #if TCA0_MS_DELAY_ENABLE

	base_tools_init();
	DBG_INIT();
	irqen();
}

//================================================//
//========= Milisecond Tick implementation =======//
//================================================//
#if MS_TICK_ENABLE == TRUE	

#ifndef TCB0
	#error
#endif

// IRQ
ISR(TCB0_INT_vect)
{
	msInc();
	TCB0.INTFLAGS = 1;
}

void msInit(void)
{
	TCB0.CCMP = F_CPU/1000;
	TCB0.CTRLA = TCB_ENABLE_bm;
	TCB0.INTCTRL = 1;
}

void msStop(void)
{
	TCB0.CTRLA = 0;
}

void msStart(void)
{
	TCB0.CTRLA = TCB_ENABLE_bm;
}

#endif //MS_TICK_ENABLE == TRUE	


//================================================//
//========= Microecond Tick implementation =======//
//================================================//
#if BASE_TOOLS_USDELAY_IMPLEMENT == TRUE

#if BASE_USDELAY_16_BIT != TRUE
	#error
#endif

#if F_CPU < 1000000ul
	#error
#endif

#ifndef US_TMR_TCB_INDEX
	#error
#endif

#if (US_TMR_TCB_INDEX<0) || (US_TMR_TCB_INDEX>2)
	#error
#endif

#if US_TMR_TCB_INDEX == 0 
	#if MS_TICK_ENABLE == TRUE
		#error "TCB0 in use"
	#endif
	#define US_TMR		TCB0			
#elif US_TMR_TCB_INDEX == 1 
	#ifndef TCB1
		#error "TCB1 not present in this processor"
	#endif
	#define US_TMR		TCB1
#else // US_TMR_TCB_INDEX = 2 
	#ifndef TCB2
		#error "TCB2 not present in this processor"
	#endif
	#define US_TMR		TCB2	
#endif

__weak void usDelayInit(void)
{
#if F_CPU == 1000000ul
	US_TMR.CTRLA = TCB_ENABLE_bm;	/* Enable timer */
#elif F_CPU == 2000000ul
	US_TMR.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL0_bm;
#elif (F_CPU == 4000000ul) || (F_CPU == 8000000ul) || (F_CPU == 16000000ul)
	// Set TCA clock source to 1Mhz
#if (F_CPU == 4000000ul) 
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc | TCA_SINGLE_ENABLE_bm;
#elif (F_CPU == 8000000ul) 
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV8_gc | TCA_SINGLE_ENABLE_bm;
#else // (F_CPU == 16000000ul) 
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm;
#endif
	US_TMR.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL1_bm;
#else
	#error "Only 1Mhz/2MHz/4Mhz/8MHz/16MHz F_CPU supported" 
#endif
}

#ifndef BASE_TOOLS_USDELAY_INLINE
	#error
#endif
#if BASE_TOOLS_USDELAY_INLINE == FALSE
__weak void usDelay(usDelay_t usec)
{
	usDelay_t start = US_TMR.CNT;
	while ( usIsTimeElapsed(start, usec) == FALSE );
}
#endif

/*
	Start/Stop us Timer
*/
__weak extern void usStart(void)
{
	US_TMR.CTRLA = TCB_ENABLE_bm;
}

__weak extern void usStop(void)
{
	US_TMR.CTRLA = 0;
}

#ifndef BASE_USGET
__weak usDelay_t  usGet(void)
{
	return US_TMR.CNT;
}
#endif

__weak void usClear(void)
{
	US_TMR.CNT = 0;
}

#endif // #if BASE_TOOLS_USDELAY_IMPLEMENT == TRUE




