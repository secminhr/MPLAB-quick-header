#include <xc.h>
    //setting TX/RX

#include "uart_tool.h"
int str_ready = 0;
char mystring[20];
int lenStr = 0;

void UART_Initialize() {
           
    /*       TODObasic   
           Serial Setting      
        1.   Setting Baud rate
        2.   choose sync/async mode 
        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
        3.5  enable Tx, Rx Interrupt(optional)
        4.   Enable Tx & RX
    */
           
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;            
    
    //  Setting baud rate
    TXSTAbits.SYNC = 0;           
    BAUDCONbits.BRG16 = 0;          
    TXSTAbits.BRGH = 0;
    SPBRG = 51;      
    
   //   Serial enable
    RCSTAbits.SPEN = 1;              
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;           
    RCSTAbits.CREN = 1;             
    PIE1bits.TXIE = 0;       
    // IPR1bits.TXIP = ;             
    PIE1bits.RCIE = 1;              
    IPR1bits.RCIP = 0;              
}

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}


void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

/*basic 
void MyusartRead()
{
    // TODObasic: try to use UART_Write to finish this function 
    unsigned char c = RCREG;
    if (c != 0xFF) {
        if (c == '\r') UART_Write('\n');
        UART_Write(c);
    }
}
*/
/*advanced 
void MyusartRead() {
    unsigned char c = RCREG;
    if (c != 0xFF) {
        if (c != '\n' && c != '\r') {
            mystring[lenStr++] = c;
        } else {
            mystring[lenStr] = '\0';
            str_ready = 1;
        }
        if (c == '\r') UART_Write('\n');
        UART_Write(c);
    }
}
*/
int in_mode = 0;
/*bonus */
void MyusartRead() {
    unsigned char c = RCREG;
    if (c != 0xFF) {
        if (in_mode) {
            if (c == 'e') {
                in_mode = 0;
                return;
            }
            if (c == '\r') UART_Write('\n');
            UART_Write(c);
        } else {
            if (c != '\n' && c != '\r') {
                mystring[lenStr++] = c;
            } else {
                mystring[lenStr] = '\0';
                str_ready = 1;
            }
            if (c == '\r') UART_Write('\n');
            UART_Write(c);
        }
    }
}

char *GetString(){
    lenStr = 0;
    while(!str_ready) ;
    str_ready = 0;
    return mystring;
}


// void interrupt low_priority Lo_ISR(void)
//void __interrupt(high_priority)  Lo_ISR(void)
//{
//    if(RCIF)
//    {
//        if(RCSTAbits.OERR)
//        {
//            CREN = 0;
//            Nop();
//            CREN = 1;
//        }
//        
//        MyusartRead();
//    }
//    
//   // process other interrupt sources here, if required
//    return;
//}