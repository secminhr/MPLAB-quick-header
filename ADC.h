#ifndef ADC_H
#define	ADC_H

#include <pic18f4520.h>
#include "OSC.h"
#include "Interrupt.h"

void useAnalogUpToAN(char anport) {
    ADCON1bits.PCFG = 0xE - anport;
}

#define ADC8BIT 0
#define ADC10BIT 1

char TADSelect[6] = {
    0b000,
    0b100,
    0b001,
    0b101,
    0b010,
    0b110
};
char TacqSelect[8] = {
    0b000,
    0b001,
    0b010,
    0b011,
    0b100,
    0b101,
    0b110,
    0b111
};
char TacqSelectFactor[8] = {
    0, 2, 4, 6, 8, 12, 16, 20
};

#define VSS 0
#define VDD 0
#define VREF 1

char adc_use_interrupt = 0;
char adc_mode = 0;

void configADC(char mode, char VPlusSource, char VMinusSource, char use_interrupt, INT_PRIORITY priority) {
    ADCON2bits.ADFM = mode;
    adc_mode = mode;
    ADCON1bits.VCFG0 = VPlusSource;
    ADCON1bits.VCFG1 = VMinusSource;
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = use_interrupt;
    IPR1bits.ADIP = priority;
    has_peripheral_interrupt |= use_interrupt;
    adc_use_interrupt = use_interrupt;
    ADCON0bits.ADON = 1;
    
    double Tosc = Tcycle_halfus() / 8.0;
    double TAD;
    for (int i = 0; i < 6; i++) {
        TAD = Tosc * (2 << i);
        if (TAD >= 0.7) {
            ADCON2bits.ADCS = TADSelect[i];
            break;
        }
    }
    //i == 0 is intentionally dropped
    for (int i = 1; i < 8; i++) {
        if (TAD * TacqSelectFactor[i] >= 2.4) {
            ADCON2bits.ACQT = TacqSelect[i];
            break;
        }
    }
}

void startConvertAN(char channel, unsigned short *res) {
    ADCON0bits.CHS = channel;
    ADCON0bits.GO = 1;
    if (!adc_use_interrupt) {
        while (ADCON0bits.NOT_DONE) ;
        if (adc_mode == ADC8BIT) {
            *res = ADRESH;
        } else {
            *res = ADRES;
        }
    }
}

#endif