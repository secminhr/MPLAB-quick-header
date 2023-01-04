#ifndef OSC_H
#define	OSC_H

#include <pic18f4520.h>

typedef enum {
    F31kHz, F125kHz, F250kHz, F500kHz, F1MHz, F2MHz, F4MHz, F8MHz
} Freq;

unsigned char OSCValue[8] = {
    0b00000010,
    0b00010010,
    0b00100010,
    0b00110010,
    0b01000010,
    0b01010010,
    0b01100010,
    0b01110010
};

//unit: 0.5us
unsigned int Tcycle[8] = {
    256, 64, 32, 16, 8, 4, 2, 1
};

Freq fosc = F1MHz;
unsigned int Tcycle_halfus() {
    return Tcycle[fosc];
}

void setOSCFreq(Freq f) {
    fosc = f;
    OSCCON = OSCValue[f];
}

#endif	/* OSC_H */

