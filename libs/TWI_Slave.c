/*
 * twislave.c
 *
 * Created:		31.05.2014 12:47:52
 * Author:		Scheik
 * Description:	C-Source f�r Headerfile twislave.h
 */

#include <util/twi.h> 														// enth�lt z.B. die Bezeichnungen f�r die Statuscodes in TWSR
#include <avr/interrupt.h>													// dient zur Behandlung der Interrupts
#include "TWI_Slave.h"



// Function init_twi_slave
void init_twi_slave(uint8_t adr)											// Initaliserung des TWI-Inteface. Muss zu Beginn aufgerufen werden, sowie bei einem Wechsel der Slave Adresse. Parameter adr = gew�nschte Slave-Adresse
{
    TWAR= adr; //Adresse setzen
	TWCR &= ~(1<<TWSTA)|(1<<TWSTO);
	TWCR|= (1<<TWEA) | (1<<TWEN)|(1<<TWIE);
	buffer_adr=0xFF;
} //End Function init_twi_slave


// ISR Routine f�r TWI Interrupts
ISR (TWI_vect)
{
uint8_t data=0;
	switch (TW_STATUS) 														//TWI-Statusregister pr�fen und n�tige Aktion bestimmen
	{
		// SLAVE RECEIVER
		case TW_SR_SLA_ACK: 												// 0x60 Slave Receiver, Slave wurde adressiert
			TWCR_ACK; 														// n�chstes Datenbyte empfangen, ACK danach senden
			buffer_adr=0xFF; 												// Bufferposition ist undefiniert
			break;
		case TW_SR_ARB_LOST_SLA_ACK:
			TWCR_ACK;
			break;
		case TW_SR_GCALL_ACK:
			TWCR_ACK;
			break;
		case TW_SR_ARB_LOST_GCALL_ACK:
			TWCR_ACK;
			break;
		case TW_SR_DATA_ACK: 												// 0x80 Slave Receiver, ein Datenbyte wurde empfangen
			data=TWDR; 														// Empfangene Daten auslesen
			if (buffer_adr == 0xFF) 										// erster Zugriff, Bufferposition setzen
				{
				if(data<i2c_buffer_size+1)									// Kontrolle ob gew�nschte Adresse im erlaubten bereich
					{
						buffer_adr= data; 									// Bufferposition wie adressiert setzen
					}
				else
					{
						buffer_adr=0; 										// Adresse auf Null setzen. Ist das sinnvoll? TO DO!
					}
				TWCR_ACK;													// n�chstes Datenbyte empfangen, ACK danach, um n�chstes Byte anzufordern
				}
			else 															// weiterer Zugriff, nachdem die Position im Buffer gesetzt wurde. NUn die Daten empfangen und speichern
				{

				if(buffer_adr<i2c_buffer_size+1)
					{
							i2cdata[buffer_adr]=data; 						// aten in Buffer schreibe
					}
				buffer_adr++; 												// Buffer-Adresse weiterz�hlen f�r n�chsten Schreibzugriff
				TWCR_ACK;
				}
			break;
		case TW_SR_DATA_NACK: 												// 0x88
			TWCR_ACK;
			break;
		case TW_SR_GCALL_DATA_ACK:
			TWCR_ACK;
			break;
		case TW_SR_GCALL_DATA_NACK:
			TWCR_ACK;
			break;
		case TW_SR_STOP:
			TWCR_ACK;
			//TWI_SR_MSG_Flag=1;
			break;
		//SLAVE TRANSMITTER
		case TW_ST_SLA_ACK: 												// 0xA8 Slave wurde im Lesemodus adressiert und hat ein ACK zur�ckgegeben.

		case TW_ST_DATA_ACK: 												// 0xB8 Slave Transmitter, Daten wurden angefordert
			if (buffer_adr == 0xFF) 										// zuvor keine Leseadresse angegeben!
				{
				buffer_adr=0;
				}

			if(buffer_adr<i2c_buffer_size+1)
				{
				TWDR = i2cdata[buffer_adr]; 								// Datenbyte senden
				buffer_adr++; 												// bufferadresse f�r n�chstes Byte weiterz�hlen
				}
			else
				{
					TWDR=0; 												// Kein Daten mehr im Buffer
				}
			TWCR_ACK;
		break;
		case TW_ST_DATA_NACK: 												// 0xC0 Keine Daten mehr gefordert
		case TW_ST_LAST_DATA: 												// 0xC8  Last data byte in TWDR has been transmitted (TWEA = ); ACK has been received
		default:
			TWCR_RESET;
		break;
	} //end.switch (TW_STATUS)
} //end.ISR(TWI_vect)

////Ende von TWI_Slave.c////
