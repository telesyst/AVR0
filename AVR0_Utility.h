/*
 *  TINY0_Utility.h
 *  Author: Nikola Kirov
 */ 
#ifndef TINY0_UTILITY_H_
#define TINY0_UTILITY_H_

static inline void sleep(SLPCTRL_SMODE_t type)
{
	SLPCTRL.CTRLA = SLPCTRL_SEN_bm | type;
	sleep_cpu();
	SLPCTRL.CTRLA = 0;
}

#define SLEEP(type)						sleep(type)

static inline void  IDLE(void)			{ sleep(SLPCTRL_SMODE_IDLE_gc); }
static inline void  STANDBY(void)		{ sleep(SLPCTRL_SMODE_STDBY_gc); }
static inline void  POWER_DOWN(void)	{ sleep(SLPCTRL_SMODE_PDOWN_gc); }

#ifdef RSTCTRL   // Have reset controler
static inline rst_cause_t reset_cause_get(void)
{
	u8 reg = RSTCTRL.RSTFR;
	if (reg & RSTCTRL_PORF_bm)
		return RST_CAUSE_POR;
	if (reg & RSTCTRL_EXTRF_bm)
		return RST_CAUSE_RST;
	if (reg & RSTCTRL_BORF_bm)
		return RST_CAUSE_BOD;
	if (reg & RSTCTRL_WDRF_bm)
		return RST_CAUSE_WD1;
	if ( reg & RSTCTRL_SWRF_bm)
	  	return RST_CAUSE_SOFT;
	return RST_CAUSE_UNKNOWN;
}
#else // Not have reset controller
static inline rst_cause_t reset_cause_get(void)
{
#ifndef MCUCSR
	u8 reg = MCUSR;
#elif defined(MCUCSR)
	u8 reg = MCUCSR;
#else
	#error
#endif
	if (reg & (1<<PORF))
		return RST_CAUSE_POR;
	if (reg & (1<<EXTRF))
		return RST_CAUSE_RST;
	if (reg & (1<<BORF))
		return RST_CAUSE_BOD;
	if (reg & (1<<WDRF))
		return RST_CAUSE_WD;
	return RST_CAUSE_UNKNOWN;
}
#endif // #ifdef RSTCTRL   // Have reset controler


#endif /* AVRM_UTILITY_H_ */