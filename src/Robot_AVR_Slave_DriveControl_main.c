/*
 *  Robot_AVR_Slave_DriveControl_main.c
 *
 *  Created on: 25.08.2014
 *  Author    : scheik.todeswache@googlemail.com
 *
 *  Description:
 *  AVR ATMega32@16MHz as slave in I2C-/TWI-Bus
 *  AVR uses internal UART(38400 8N1) for serial communication with MD49, driving to geared drives with Encoders
 *
 */

#include <stdlib.h>
#include <avr/io.h>									// Standard-Headerdatei(avr-libc): IO- Definitionen f�r gew�hlten Controller. (hier -mmcu=atmega32)
#include <util/twi.h> 	    						// enthaelt z.B. die Bezeichnungen fuer die Statuscodes in TWSR
#include <avr/interrupt.h>  						// dient zur Behandlung der Interrupts
#include <avr/pgmspace.h>							// Standard-Headerdatei(avr-libc): Funktionen in diesem Modul erm�glichen Zugriff auf Daten aus dem Flash-Memory
#include "global.h"									// globale Deklarationen
#include "TWI_Slave.h"								// Header f�r TWI_Slave Funktionsbibliothek
#include "rs232_noInterrupt.h"						// Headerdatei für UART Funktionen
#include "md49_lib.h"								// Headerdatei für MD49 Funktionen

#define SLAVE_ADRESSE 0x50 							// Die eigene Slave-Adresse

int main(void) {
	init_uart();									// UART initalisieren und einschalten. Alle n�tigen Schnittstellen-Parameter und -Funktionen werden in rs232.h definiert
	init_MD49data();								// set defaults for MD49 data and commands
	init_twi_slave(SLAVE_ADRESSE);					// Init AVR as Slave with Adresse SLAVE_ADRESSE
	sei();											// Interrupts enabled
	resetEncoders();								// Reset the encoder values to 0
	while (1)										// mainloop
	{
		setMD49commands();							// set commands on MD49 corresponding to values stored in i2cdata(0-14)
		getMD49data();								// get all data from MD49 and save to corresponding values in i2cdata(15-32)
	} //end.mainloop
} //end.mainfunction

