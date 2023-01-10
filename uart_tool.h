/* 
 * File:   uart_tool.h
 * Author: secminhr
 *
 * Created on 2023?1?5?, ?? 5:31
 */

#ifndef UART_TOOL_H
#define	UART_TOOL_H
      
void UART_Initialize(void);
char *GetString(void);
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer();
void MyusartRead();

/*bonus*/
extern int in_mode;

#endif

