/*
 * TWI_Slave.h
 *
 *  Created on: 25.08.2014
 *      Author: Scheik
 */

#ifndef TWI_SLAVE_H_
#define TWI_SLAVE_H_

#include <util/twi.h> 														// enth�lt z.B. die Bezeichnungen f�r die Statuscodes in TWSR
#include <avr/interrupt.h>													// dient zur Behandlung der Interrupts

// Vom Benutzer konfigurierbare Einstellungen
#define i2c_buffer_size 33													// Gr�sse des Buffers in Byte (2..254)

//Je nach Statuscode in TWSR m�ssen verschiedene Bitmuster in TWCR geschreiben werden(siehe Tabellen im Datenblatt!).
//Makros f�r die verwendeten Bitmuster:

//ACK nach empfangenen Daten senden/ ACK nach gesendeten Daten erwarten
#define TWCR_ACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
//NACK nach empfangenen Daten senden/ NACK nach gesendeten Daten erwarten
#define TWCR_NACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
//switch to the non adressed slave mode...
#define TWCR_RESET TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);


// Function init_twi_slave
void init_twi_slave(uint8_t adr);											// Initaliserung des TWI-Inteface. Muss zu Beginn aufgerufen werden, sowie bei einem Wechsel der Slave Adresse. Parameter adr = gew�nschte Slave-Adresse

// Fehlerbehandlung
// Bei zu alten AVR-GCC-Versionen werden die Interrupts anders genutzt, daher in diesem Fall mit einer Fehlermeldung abbrechen
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4.5 or later, update to newer AVR-GCC compiler !"
#endif

// Schutz vor unsinnigen Buffergr�ssen
#if (i2c_buffer_size > 254)
	#error Buffer zu gross gewaehlt! Maximal 254 Bytes erlaubt.
#endif
#if (i2c_buffer_size < 2)
	#error Buffer muss mindestens zwei Byte gross sein!
#endif

#endif /* TWI_SLAVE_H_ */
