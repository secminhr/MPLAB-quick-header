#ifndef TIMER_H
#define	TIMER_H

#include <pic18f4520.h>
#include "OSC.h"
#include "Interrupt.h"

#define MODE_8BIT 1
#define MODE_16BIT 0

typedef enum {
    PRE1, PRE2, PRE4, PRE8, PRE16, PRE32, PRE64, PRE128, PRE256
} CommonPrescaler;

CommonPrescaler t0pre = PRE1;

void configTimer0(char bit_mode, CommonPrescaler prescaler, char use_interrupt) {
    T0CONbits.T08BIT = bit_mode;
    t0pre = prescaler;
    if (prescaler == PRE1) {
        T0CONbits.PSA = 1;
    } else {
        T0CONbits.PSA = 0;
        T0CONbits.T0PS = prescaler - 1;
    }
    INTCONbits.TMR0IF = 0;
    
    INTCONbits.TMR0IE = use_interrupt;
    has_peripheral_interrupt |= use_interrupt;
}

unsigned int prescaler_to_num[9] = {
    1, 2, 4, 8, 16, 32, 64, 128, 256
};

//How many round needed is returned. 
//If the time can be finished in one time, corresponding TMR0 is set and 1 is returned
//If TMR0 isn't enough, it'll be set to 0 and how many rounds of max count is needed is returned
int setTimeout0(unsigned long long us) {
    unsigned long long counter = us * 2 / Tcycle_halfus() / prescaler_to_num[t0pre]; 
    if (T0CONbits.T08BIT == MODE_8BIT) {
        if (counter <= 0x100) {
            TMR0L = 0x100 - counter;
            return 1;
        } else {
            TMR0L = 0;
            return (counter + 0xFE) / 0xFF; //round up
        }
    } else {
        if (counter <= 0x10000) {
            TMR0H = (0x10000 - counter) >> 8;
            TMR0L = 0x10000 - counter;
            return 1;
        } else {
            TMR0H = 0;
            TMR0L = 0;
            return (counter + 0xFFFE) / 0xFFFF;
        }
    }
}

void startTimer0() {
    T0CONbits.TMR0ON = 1;
}

void stopTimer0() {
    T0CONbits.TMR0ON = 0;
}

CommonPrescaler t1pre = PRE1;
void configTimer1(CommonPrescaler prescaler, char use_interrupt, INT_PRIORITY priority) {
    T1CON = 0b10000000;
    T1CONbits.T1CKPS = prescaler;
    t1pre = prescaler;
    
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = use_interrupt;
    IPR1bits.TMR1IP = priority;
    has_peripheral_interrupt |= use_interrupt;
}

int setTimeout1(unsigned long long us) {
    unsigned long long counter = us * 2 / Tcycle_halfus() / prescaler_to_num[t1pre]; 
    if (counter <= 0x10000) {
        TMR1H = (0x10000 - counter) >> 8;
        TMR1L = 0x10000 - counter;
        return 1;
    } else {
        TMR1H = 0;
        TMR1L = 0;
        return (counter + 0xFFFE) / 0xFFFF;
    }
}

void startTimer1() {
    T1CONbits.TMR1ON = 1;
}

void stopTimer1() {
    T1CONbits.TMR1ON = 0;
}

CommonPrescaler t3pre = PRE1;
void configTimer3(CommonPrescaler prescaler, char use_interrupt, INT_PRIORITY priority) {
    T3CON = 0b10000000;
    T3CONbits.T3CKPS = prescaler;
    t3pre = prescaler;
    
    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = use_interrupt;
    IPR2bits.TMR3IP = priority;
    has_peripheral_interrupt |= use_interrupt;
}

int setTimeout3(unsigned long long us) {
    unsigned long long counter = us * 2 / Tcycle_halfus() / prescaler_to_num[t1pre]; 
    if (counter <= 0x10000) {
        TMR3H = (0x10000 - counter) >> 8;
        TMR3L = 0x10000 - counter;
        return 1;
    } else {
        TMR3H = 0;
        TMR3L = 0;
        return (counter + 0xFFFE) / 0xFFFF;
    }
}

void startTimer3() {
    T3CONbits.TMR3ON = 1;
}

void stopTimer3() {
    T3CONbits.TMR3ON = 0;
}

typedef enum {
    T2PRE1, T2PRE4, T2PRE16
} T2Prescaler;

int t2_prescaler_to_num[3] = { 1, 4, 16 };

T2Prescaler t2pre = PRE1;
char t2post = 1;

void configTimer2(T2Prescaler prescaler, char post, char use_interrupt, INT_PRIORITY priority) {
    T2CON = 0;
    T2CONbits.T2CKPS = prescaler;
    t2pre = prescaler;
    T2CONbits.T2OUTPS = post - 1;
    t2post = post;
    
    
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = use_interrupt;
    IPR1bits.TMR2IP = priority;
    has_peripheral_interrupt |= use_interrupt;
}

int setTimeout2(unsigned long long us) {
    unsigned long long counter = us * 2 / Tcycle_halfus() / t2_prescaler_to_num[t2pre] / t2post; 
    if (counter <= 0x100) {
        PR2 = counter - 1;
        TMR2 = 0;
        return 1;
    } else {
        PR2 = 0xFF;
        TMR2 = 0;
        return (counter + 0xFE) / 0xFF;
    }
}

void startTimer2() {
    T2CONbits.TMR2ON = 1;
}

void stopTimer2() {
    T2CONbits.TMR2ON = 0;
}

#endif

