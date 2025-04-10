#ifndef _COMPILR_PECIFIC_H_
#define _COMPILR_PECIFIC_H_

#ifdef __GNUC__
  //#include <compiler.h>
  #define __weak						__attribute__ ((weak))
  #define ALLOCA_ENABLE		     		TRUE
  #include <alloca.h>
  #define OPTIMIZE_HIGH					__attribute__((optimize("s"))) 
#else //==== IAR ====
  #define __weak                 		__attribute__ ((weak))
  #define ALLOCA_ENABLE		     		FALSE
  #define OPTIMIZE_HIGH					_Pragma("optimize=speed")
  #define pgm_read_ptr(address_ptr)    	(void*)(*(unsigned __flash *)(address_ptr))
  #define __builtin_avr_delay_cycles	__delay_cycles
#endif

#ifndef NO_INIT
	#define NO_INIT(a)		     __attribute__((section(".no_init")))
#endif

/*
#if defined(NO_ASF_PROJECT)



#ifndef __always_inline
	#define __always_inline		inline __attribute__((__always_inline__))
#endif

#ifndef __no_inline
	#define __no_inline			__attribute__((__noinline__))
#endif

	#define WEAK				__attribute__ ((weak))
	#define RAMFUNCL			__attribute__ ((long_call, section (".ramfunc")))
	#define RAMFUNC				__attribute__ ((section(".ramfunc")))
	
	#define OPTIMIZE_HIGH		__attribute__((optimize("s")))
#endif
#endif
*/

#endif // #ifndef _COMPILR_PECIFIC_H_