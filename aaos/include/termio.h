/** \file termio.h
	Terminal I/O functions.
*/
#ifndef _TERMIO_H
#define _TERMIO_H

/** \defgroup termio Terminal I/O 
	termio.h: Set of functions for managing I/O operations from/to
	terminals.
	
	NOTE: while one of these function is using a terminal, others call
	to the same terminal may fail.
\{ */

/** Terminal 0 address. */
#define TERMINAL0ADDR 0x10000250

/** Maximum number of terminals. */
#define MAX_TERMINALS 8

/** Terminal "print character" command. */
#define PRINTCHAR 2

/** \internal */
#define CHAROFFSET 8

/** \internal */
#define TRANSMITTED 5

/** Device register field number for terminal devices. */
enum TerminalRegisters {
	RECVSTATUS,
	RECVCOMMAND,
	TRANSTATUS,
	TRANCOMMAND
};

/** Prints a character on specified terminal.
	\param term The output terminal.
	\param c The character to print.
	\returns The number of characters printed, if any, -1 on error.
*/
int term_putchar(int term, int c);

/** Prints a string on specified terminal.
	\param term The output terminal.
	\param str The string to print.
	\returns The number of characters printed, if any, -1 on error.
*/
int term_puts(int term, const char *str);

/** Prints a formatted string on specified terminal.
	This is a printf-like function: recognize the following special characters:
	- %%d Print a signed decimal (integer);
	- %%s Print a string;
	- %%c Print a character;
	- %%b Print an integer in binary form;
	- %%o Print an integer in octal form;
	- %%x or %%X Print an integer in hexadecimal form;
	- %% Print the character %.
	\param term The output terminal.
	\param fmt The formatted string.
	\returns The number of characters printed, if any, -1 on error.
*/
int term_printf(int term, const char *fmt, ...);

/** Read a character from the specified terminal.
	\param term The input terminal.
	\returns The character red, if any, -1 on error.
*/
int term_getc(int term);

/** Read a string form terminal.
	NOT IMPLEMENTED YET.
	
	Reads in at most one less than size characters from stream  and
	stores  them  into  the buffer pointed to by s.
	\param term The input terminal.
	\param s The input buffer.
	\param size The size of input buffer.
	\returns On success s, NULL otherwise.
*/
char* term_gets(int term, char *s, int size);

/** Input format conversion.
	NOT IMPLEMENTED YET.
	
	\param term The output terminal.
	\param fmt The formatted string.
	\returns The number of input items  successfully  matched
		and assigned, which can be fewer than provided for, or even zero
		in the event of an early matching failure. -1 on error.
*/
int term_scanf(int term, const char *fmt, ...);
/** \} */

#endif
