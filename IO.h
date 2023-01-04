#ifndef IO_H
#define IO_H
#include <pic18f4520.h>

#define HIGH 1
#define LOW 0
#define IN 1
#define OUT 0

#define setIOMode(port, pin, mode) do { \
		ADCON1 = 0x0F; \
		TRIS ## port ## bits.R ## port ## pin = mode; \
	} while (0)

#define write(port, pin, out) LAT ## port ## bits.L ## port ## pin = out
#define read(port, pin) PORT ## port ## bits.R ## port ## pin

#endif