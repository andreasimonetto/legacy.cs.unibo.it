#include "termio.h"
#include "errno.h"
#include "arch.h"
#include "math.h"
#include "kernel.h"
#include <stdarg.h>

int errno = 0;

int term_putuint(int term, unsigned n, unsigned base);

int term_putchar(int term, int c)
{
volatile unsigned *statusp;
volatile unsigned *commandp;
unsigned stat;

	if(term < 0 || term > MAX_TERMINALS) {
		/* Wrong terminal device number */
		return ETERM_WRONGID;
	}

	/* Compute device register field addresses */
	statusp = (unsigned*) (TERMINAL0ADDR + (term * DEVREGSIZE) + (TRANSTATUS * DEVREGLEN));
	commandp = (unsigned*) (TERMINAL0ADDR + (term * DEVREGSIZE) + (TRANCOMMAND * DEVREGLEN));
	
	/* Retrieve device status */
	stat = ((*statusp) & STATUSMASK);

	/* If device is available */
	if(stat == READY || stat == TRANSMITTED) {
		/* Now we shift the char to be printed by CHAROFFSET to 
		 * put its bits to the 8 higher bits and set the command code at 2*/
		*commandp = (c << CHAROFFSET) | PRINTCHAR;

		if(getSTATUS() & 0x1) {
			/* If interrupts are enabled, perform SSI request */
			SSIRequest(WAITFORIO, (unsigned)statusp, &stat);
		}
		else {
			/* do busy waiting */
			do 
				stat = ((*statusp) & STATUSMASK);
			while (stat == BUSY);
				
			if(stat == TRANSMITTED)
				*commandp = ACK;
		}

		if(stat != TRANSMITTED)
			return ETERM_UNAVAIL;
	}
	else {
		/* Device is not available */
		return ETERM_UNAVAIL;
	}

	/**commandp = ACK;*/
	return c;
}

int term_puts(int term, const char *str)
{
const char *cur_ptr = str;

	/* During this cycle we will print one char a time */
	while(*cur_ptr) {
		if(term_putchar(term, *cur_ptr) == -1)
			return -1;
			
		/* Move to next char */
		cur_ptr++;
	}

	return (cur_ptr - str);
}

int term_putint(int term, int n)
{
	if(n < 0) {
		n = abs(n);
		if(term_putchar(term, '-') == -1)
			return -1;
		return term_putuint(term, n, 10) + 1;
	}
	
	return term_putuint(term, n, 10);
}

int term_printf(int term, const char *fmt, ...)
{
va_list ap;
int err, printed = 0;
char c;

	va_start(ap, fmt);
	while(*fmt) {
		if((c = *fmt++) == '%') {
			switch((c = *fmt++)) {
				case 's': /* string */
					printed += (err = term_puts(term, va_arg(ap, const char*)));
					break;
				case 'd': /* int */
					printed += (err = term_putint(term, va_arg(ap, int)));
					break;
				case 'c': /* char */
					/* need a cast here since va_arg only
					takes fully promoted types */
					err = term_putchar(term, (char) va_arg(ap, int));
					printed++;
					break;
				case 'b': /* bin */
					printed += (err = term_putuint(term, va_arg(ap, unsigned), 2));
					break;
				case 'o': /* oct */
					printed += (err = term_putuint(term, va_arg(ap, unsigned), 8));
					break;
				case 'x': /* hex */
				case 'X':
					printed += (err = term_putuint(term, va_arg(ap, unsigned), 16));
					break;
				default:
					err = term_putchar(term, c);
					printed++;
					break;
			}
		}
		else {
			err = term_putchar(term, c);
			printed++;
		}
		
		if(err == -1)
			return -1;
	}
	
	va_end(ap);
	return printed;
}

int term_putuint(int term, unsigned n, unsigned base)
{
int printed = 0;
unsigned exp = 1;
unsigned digit;
int err;

	while((n / exp) >= base) {
		exp *= base;
	}
	
	while(exp > 0) {
		digit = (n / exp);
		if(digit < 10)
			err = term_putchar(term, '0' + digit);
		else
			err = term_putchar(term, 'A' + digit - 10);
		
		if(err == -1)
			return -1;
		
		n %= exp;
		exp /= base;
		printed++;
	}
	
	return printed;
}
