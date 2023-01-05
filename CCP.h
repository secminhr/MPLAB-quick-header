#ifndef CCP_H
#define	CCP_H

#include <pic18f4520.h>
#include "IO.h"
#include "Interrupt.h"

#define TIMER1 0
#define TIMER3 1

void configCCPTimer(char ccp1, char ccp2) {
    //a bit counter-intuitive
    //T3CCP2 is used to control ccp1, while T3CCP1 is used to control ccp2
    //0 indicates timer1, 1 indicates timer 3
    T3CONbits.T3CCP2 = ccp1;
    T3CONbits.T3CCP1 = ccp2;
}

typedef enum {
    CAP_FALLING = 4, CAP_RISING, CAP_RISING4, CAP_RISING16
} CaptureEvent;

void configCaptureCCP(int ccp, CaptureEvent event, char use_interrupt, INT_PRIORITY priority) {
    if (ccp == 1) {
        setIOMode(C, 2, IN);
        CCP1CONbits.CCP1M = event;
        PIR1bits.CCP1IF = 0;
        PIE1bits.CCP1IE = use_interrupt;
        IPR1bits.CCP1IP = priority;
    } else {
        setIOMode(C, 3, IN);
        CCP2CONbits.CCP2M = event;
        PIR2bits.CCP2IF = 0;
        PIE2bits.CCP2IE = use_interrupt;
        IPR2bits.CCP2IP = priority;
    }
    has_peripheral_interrupt |= use_interrupt;
}

//see special event in datasheet
typedef enum {
    TOGGLE = 2, GO_HIGH = 8, GO_LOW, COMP_INTERRUPT, SPEVENT
} CompareEvent;

//priority is used with COMP_INTERRUPT
void configCompareCCP(int ccp, unsigned short match_value, CompareEvent event, INT_PRIORITY priority) {
    if (ccp == 1) {
        setIOMode(C, 2, OUT);
        CCPR1 = match_value;
        CCP1CONbits.CCP1M = event;
        PIR1bits.CCP1IF = 0;
        if (event == COMP_INTERRUPT) {
            PIE1bits.CCP1IE = 1;
            IPR1bits.CCP1IP = priority;
            has_peripheral_interrupt |= 1;
        }
    } else {
        setIOMode(C, 3, OUT);
        CCPR2 = match_value;
        CCP2CONbits.CCP2M = event;
        PIR2bits.CCP2IF = 0;
        if (event == COMP_INTERRUPT) {
            PIE1bits.CCP1IE = 1;
            IPR1bits.CCP1IP = priority;
            has_peripheral_interrupt |= 1;
        }
    }
}

void configPWMCCP(int ccp, unsigned long long period, int duty_cycle_percentage) {
    double val = 1.0 * PR2 * duty_cycle_percentage / 100;
    if (ccp == 1) {
        setIOMode(C, 2, OUT);
        CCPR1L = (unsigned char) val;
        CCP1CONbits.DC1B = (unsigned) (4 * (val - CCPR1L));
        CCP1CONbits.CCP1M = 0xC0;
    } else {
        setIOMode(C, 3, OUT);
        CCPR2L = (unsigned char) val;
        CCP2CONbits.DC2B = (unsigned) (4 * (val - CCPR2L));
        CCP2CONbits.CCP2M = 0xC0;
    }
    configTimer2(16, POST1, 0, HIGH_PRIORITY);
    setTimeout2(period);
}

void startPWM() {
    startTimer2();
}

#endif	/* CCP_H */

