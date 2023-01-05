#ifndef UART_H
#define	UART_H

#include "IO.h"
#include "Interrupt.h"

//return whether the desired baud_rate is possible
//1 if set, 0 if failed
int configUART(int BRG16, int BRGH, unsigned long long baud_rate, 
               int tx_interrupt, INT_PRIORITY tx_pri, int rx_interrupt, INT_PRIORITY rx_pri) {
    if (!BRG16 && !BRGH) {
        unsigned long long val = Fosc_number() >> 6;
        if (val / 256 <= baud_rate && baud_rate <= val) {
            SPBRG = val / baud_rate - 1;
        } else {
            return 0;
        }
    } else if(BRG16 ^ BRGH) {
        unsigned long long val = Fosc_number() >> 4;
        unsigned long long reg_max = BRG16 ? 65536ull : 256ull;
        
        if (val / reg_max <= baud_rate && baud_rate <= val) {
            if (BRG16) {
                unsigned long long brg = val / baud_rate - 1;
                SPBRG = brg;
                SPBRGH = brg >> 8;
            } else {
                SPBRG = val / baud_rate - 1;
            }
        } else {
            return 0;
        }
    } else {
        unsigned long long val = Fosc_number() >> 2;
        if (val / 65536ull <= baud_rate && baud_rate <= val) {
            unsigned long long brg = val / baud_rate - 1;
            SPBRG = brg;
            SPBRGH = brg >> 8;
        } else {
            return 0;
        }
    }
    
    setIOMode(C, 6, IN);
    setIOMode(C, 7, IN);
    TXSTAbits.SYNC = 0; //async
    BAUDCONbits.BRG16 = BRG16;          
    TXSTAbits.BRGH = BRGH;
    
    RCSTAbits.SPEN = 1;              
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;           
    RCSTAbits.CREN = 1; 
    
    IPR1bits.TXIP = tx_pri;
    IPR1bits.RCIP = rx_pri;
    PIE1bits.TXIE = tx_interrupt;
    PIE1bits.RCIE = rx_interrupt;
    
    has_peripheral_interrupt |= (tx_interrupt | rx_interrupt);
    
    return 1;
}

#endif	/* UART_H */

