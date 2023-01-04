#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "IO.h"

typedef enum {
	FALLING = 0,
	RISING
} INT_EDGE;

typedef enum {
    LOW_PRIORITY = 0,
    HIGH_PRIORITY
} INT_PRIORITY;

char has_peripheral_interrupt = 0;

void configINT(int inter, INT_EDGE edge, INT_PRIORITY pri) {
	if (inter == 0) {
        setIOMode(B, 0, IN);
        INTCON2bits.INTEDG0 = edge;
        INTCONbits.INT0IF = 0;
        INTCONbits.INT0IE = 1;
    } else if (inter == 1) {
        setIOMode(B, 1, IN);
        INTCON2bits.INTEDG1 = edge;
        INTCON3bits.INT1IF = 0;
        INTCON3bits.INT1IE = 1;
        INTCON3bits.INT1IP = pri;
    } else {
        setIOMode(B, 2, IN);
        INTCON2bits.INTEDG2 = edge;
        INTCON3bits.INT2IF = 0;
        INTCON3bits.INT2IE = 1;
        INTCON3bits.INT2IP = pri;
    }
}

void disableINT(int inter) {
    if (inter == 0) {
        INTCONbits.INT0E = 0;
    } else if (inter == 1) {
        INTCON3bits.INT1E = 0;
    } else {
        INTCON3bits.INT2E = 0;
    }
}

void interruptHandled(int inter) {
    if (inter == 0) {
        INTCONbits.INT0IF = 0;
    } else if (inter == 1) {
        INTCON3bits.INT1IF = 0;
    } else {
        INTCON3bits.INT2IF = 0;
    }
}

#define WITH_PRIORITY 1
#define WITHOUT_PRIORITY 0

void startInterrupts(int usePriority) {
    if (has_peripheral_interrupt) {
        INTCONbits.PEIE = 1;
    }
    if (usePriority) {
        RCONbits.IPEN = 1;
        INTCONbits.GIEH = 1;
        INTCONbits.GIEL = 1;
    } else {
        RCONbits.IPEN = 0;
        INTCONbits.GIE = 1;
    }
}

#endif