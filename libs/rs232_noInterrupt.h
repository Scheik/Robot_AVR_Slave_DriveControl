/*
 * rs232_noInterrupt.h
 *
 *  Created on: 26.08.2014
 *      Author: fabian
 */

#ifndef RS232_NOINTERRUPT_H_
#define RS232_NOINTERRUPT_H_


/* Schnittstellenparameter              */
/****************************************/
#define UART_BUFFER_SIZE 16													// UART-Puffer- Größe, in Bytes
#define UART_BAUDRATE 38400UL												// RS232 Baudrate
#define BAUD_PRESCALE (((F_CPU / (UART_BAUDRATE * 16UL))) - 1)
#define UBRR_VAL ((F_CPU+UART_BAUDRATE*8)/(UART_BAUDRATE*16)-1)				// runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))									// Reale Baudrate bei gegebener Taktfrequenz (FCPU) ermitteln
#define BAUD_ERROR ((BAUD_REAL*1000)/UART_BAUDRATE)							// Fehler in Promille ermitteln
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
	#error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! // Ist der Fehler > 1% gibt der Compiler einen Fehler aus
#endif
#define CR "\r\n"															// Ein Zeilenumbruch, abhängig davon, was die Gegenstelle haben will, hier für Windows : "\r\n"
																			// Linux  : "n"
																			// MacOS  : "r"
/* Funktionen und Variablen in rs232.c */
/***************************************/
//extern volatile uint8_t UART_MSG_FLAG;										// Variable UART_MSG_FLAG=1, wenn letzes empfangenes Zeichen ein Zeilenumbruch war oder UART_Puffer voll ist
//xtern volatile unsigned char UART_RXBuffer[UART_BUFFER_SIZE];				// Ringpuffer- Array für empfangene	UART Daten (UART-Puffer)
//extern volatile unsigned char UART_RxCount ;								// Zählervariable für UART-Puffer

extern void init_uart();													// Funktion in RS232.c zum Initalisieren und Aktivieren des UART mit oben eingestellten Werten
extern int uart_putc (unsigned char c);										// Funktion in RS232.c zum Senden von Bytes
extern void uart_puts (char *s);											// Funktion in RS232.c zum Senden von Strings
extern void uart_puts_p (const char *progmem_s );							// Funktion in RS232.c zum Senden von Strings aus dem Flash-Memory
extern void uart_puti (const int val);										// Funktion in RS232.c zum Senden von Zahlen
extern uint8_t uart_getc(void);												// Funktion in RS232.c zum Empfangen eines Zeichens


#endif /* RS232_NOINTERRUPT_H_ */
