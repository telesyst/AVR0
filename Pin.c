#include "pbase.h"

PORT_t * gpio_get_port(pin_t pin)
{
	PORT_t * ports[] = {
		&PORTA
#ifdef PORTB
		,&PORTB	
#endif
#ifdef PORTC
		,&PORTC	
#endif
#ifdef PORTD
		,&PORTD		
#endif
	};
	return ports[gpio_get_port_index(pin)];
}

void pbs(pin_t pin)
{
    gpio_get_port(pin)->OUTSET = (u8)getPinMask(pin);
}

void pbc(pin_t pin)
{
    gpio_get_port(pin)->OUTCLR = (u8)getPinMask(pin);
}

void pbt(pin_t pin)
{
    gpio_get_port(pin)->OUTTGL = (u8)getPinMask(pin);
}

void pbpu(pin_t pin, u8 pullup)
{
	u8 p = getPinIndex(pin);
	register8_t * reg0 = &(gpio_get_port(pin)->PIN0CTRL);
    if(pullup)
        reg0[p] |= PORT_PULLUPEN_bm;
    else
         reg0[p] &= ~PORT_PULLUPEN_bm;
}

void pbpue(pin_t pin)
{
	pbpu(pin, TRUE);
}

void pbpud(pin_t pin)
{
	pbpu(pin, FALSE);
}

void pbd(pin_t pin, u8 dir_out)
{
	if(dir_out)
		gpio_get_port(pin)->DIRSET = (u8)getPinMask(pin);
	else
		gpio_get_port(pin)->DIRCLR = (u8)getPinMask(pin);
}

void pbdo(pin_t pin)
{
	pbd(pin, TRUE);
}

void pbdi(pin_t pin)
{
	pbd(pin, FALSE);
}

void pbss(pin_t pin, u8 state)
{
    if(state)
		pbs(pin);
	else
		pbc(pin);
}

ut pbg(pin_t pin)
{
	return gpio_get_port(pin)->IN & getPinMask(pin);
}

ut pbog(pin_t pin)
{
	return gpio_get_port(pin)->OUT & getPinMask(pin);
}