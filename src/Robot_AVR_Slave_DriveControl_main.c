/*
 *  Robot_AVR_Slave_DriveControl_main.c
 *
 *  Created on: 25.08.2014
 *  Author    : scheik.todeswache@googlemail.com
 *
 *  Description:
 *  AVR ATMega32@16MHz as slave in I2C-/TWI-Bus
 *  AVR uses internal UART for serial coms with MD49
 *
 */

#include <stdlib.h>
#include <avr/io.h>									// Standard-Headerdatei(avr-libc): IO- Definitionen f�r gew�hlten Controller. (hier -mmcu=atmega32)
#include <util/twi.h> 	    						// enthaelt z.B. die Bezeichnungen fuer die Statuscodes in TWSR
#include <avr/interrupt.h>  						// dient zur Behandlung der Interrupts
#include <avr/pgmspace.h>							// Standard-Headerdatei(avr-libc): Funktionen in diesem Modul erm�glichen Zugriff auf Daten aus dem Flash-Memory
#include "global.h"									// globale Deklarationen
#include "TWI_Slave.h"								// Header f�r TWI_Slave Funktionsbibliothek
#include "rs232_noInterrupt.h"						// Headerdatei f�r rs232_noInterrupt.c

//predeclare functions
void setMode(char mode);
void resetEncoders(void);
//void driveMotors(char speed);						//is going to be deleted, later exchanged by functions setSpeed1() & setSpeed2()
void readEncoderValues(void);
//void drivetest(void);								//Testroutine, is going to get deleted soon
//new functions, not tested yet
void getSpeed1(void);
void getSpeed2(void);
void getVolts(void);
void getCurrent1(void);
void getCurrent1(void);
void getCurrent2(void);
void getAcceleration(void);
void getMode(void);
void getError(void);
void setSpeed1(char Speed1);
void setSpeed2(char Speed2);
void setAcceleration(char Acceleration);
void disableRegulator(void);
void enableRegulator(void);
void disableTimeout(void);
void enableTimeout(void);
void init_MD49data(void);
void setMD49data(void);

volatile char statusRegulator;					// stores state of regulator 0=off 1=on
volatile char statusTimeout;						// stores state of timeout 0=off 1=on
volatile char recentMode;						// stores mode recently set
volatile char recentAcceleration;				// stores acceleration recently set
volatile char recentSpeed1;						// stores recent set Speed1
volatile char recentSpeed2;						// stores recent set Speed2
//volatile uint8_t RXBuffer[8];
//volatile int32_t encoder1, encoder2;             	// To store the encoder values

#define SLAVE_ADRESSE 0x50 							// Die eigene Slave-Adresse


int main (void)
{
	init_twi_slave(SLAVE_ADRESSE);					// Init AVR as Slave with Adresse SLAVE_ADRESSE
	init_uart();									// UART initalisieren und einschalten. Alle n�tigen Schnittstellen-Parameter und -Funktionen werden in rs232.h definiert
	sei();											// Interrupts enabled
	resetEncoders();								// Reset the encoder values to 0
	init_MD49data();									// set defaults for MD49

	while(1)										// main.loop
	{
		setMD49data();								// set data on MD49 corresponding to values stored in i2cdata(array)
		//getMD49data();							// get data from MD49 and save to corresponding values in i2cdata
	} //end.while
} //end.main

void setMode(char mode)
{
	uart_putc(0);
	uart_putc(0x34);								// Command to set mode
	uart_putc(mode);								// Mode we wish to set
}

void resetEncoders(void)
{
	uart_putc(0);
	uart_putc(0x35);								// Command to reset encoder values
}

void readEncoderValues(void)
{
	uart_putc(0);
	uart_putc(0x25);								// Command to return encoder values
	//read 8 bytes
	uint8_t i;
	for (i=15;i<23;i++)
	{
		i2cdata[i] = uart_getc();
		//RXBuffer[i]=uart_getc();
	}

	// combine int32 out of 4 int8
	//encoder1 = ((int32_t) RXBuffer[0] << 24);       // Put together first encoder value
	//encoder1 |= ((int32_t) RXBuffer[1] << 16);
	//encoder1 |= ((int32_t) RXBuffer[2] << 8);
	//encoder1 |= ((int32_t) RXBuffer[3]);

	//encoder2 = ((int32_t) RXBuffer[4] << 24);       // Put together second decoder value
	//encoder2 |= ((int32_t) RXBuffer[5] << 16);
	//encoder2 |= ((int32_t) RXBuffer[6] << 8);
	//encoder2 |= ((int32_t) RXBuffer[7]);

}

void getSpeed1(void)
{

}

void getSpeed2(void)
{

}

void getVolts(void)
{

}

void getCurrent1(void)
{

}

void getCurrent2(void)
{

}

void getAcceleration(void)
{

}

void getMode(void)
{

}

void getError(void)
{

}

void setSpeed1(char Speed1)
{
	uart_putc(0);
	uart_putc(0x31);								// Command to set motor1 speed1
	uart_putc(Speed1);								// Speed to be set
	recentSpeed1=Speed1;							// save recent set Speed1
}

void setSpeed2(char Speed2)
{
	uart_putc(0);
	uart_putc(0x32);								// Command to set motor2 speed2
	uart_putc(Speed2);								// Speed to be set
	recentSpeed2=Speed2;							// save recent set Speed2
}

void setAcceleration(char Acceleration)
{
	uart_putc(0);
	uart_putc(0x33);								// Command to set acceleration
	uart_putc(Acceleration);						// Acceleration to be set
	recentAcceleration=Acceleration;				// save recent acceleration
}

void disableRegulator(void)
{
	uart_putc(0);
	uart_putc(0x36);								// Command to disable regulator (power output not changed by encoder feedback)
	statusRegulator=0;								// save recent state of regulator (disabled)
}

void enableRegulator(void)
{
	uart_putc(0);
	uart_putc(0x37);								// Command to enable regulator (power output is regulated by encoder feedback)
	statusRegulator=1;								// save recent state of regulator (enabled)
}

void disableTimeout(void)
{
	uart_putc(0);
	uart_putc(0x38);								// Command to disable Timeout (MD49 will continuously output with no regular commands)
	statusTimeout=0;								// save recent state of timeout (disabled)
}

void enableTimeout(void)
{
	uart_putc(0);
	uart_putc(0x39);								// Command to enable Timeout (MD49 output will stop after 2 seconds without communications)
	statusTimeout=1;								// save recent state of timeout (enabled)
}

void init_MD49data(void)
{
	// init bytes 0-14 (Commands)
	i2cdata[0]=128;									// set i2cdata default for speed1=0
	recentSpeed1=i2cdata[0];						// save last set speed1
	i2cdata[1]=128;									// default speed2=0#
	recentSpeed2=i2cdata[1];						// save last set speed2
	i2cdata[2]=5;									// default acceleration=5
	recentAcceleration=i2cdata[2];					// set last set acceleration
	i2cdata[3]=0;									// default mode=0
	recentMode=i2cdata[3];							// set last set Mode
	setMode(0);										// set default mode
	i2cdata[4]=0;									// default reset encoders=0
	i2cdata[5]=1;									// default enabled regulator
	enableRegulator();								// set default regulator=enabled
	//disableRegulator();
	statusRegulator=i2cdata[5];						// set state of regulator
	i2cdata[6]=0;									// default enabled timeout
	//enableTimeout();								// set default to timeout=enabled
	disableTimeout();								// set default to timeout=disabled
	statusTimeout=i2cdata[6];						// set state of timeout
	i2cdata[7]=0;									// default encoder1 value byte 1
	i2cdata[8]=0;									// default encoder1 value byte 2
	i2cdata[9]=0;									// default encoder1 value byte 3
	i2cdata[10]=0;									// default encoder1 value byte 4
	i2cdata[11]=0;									// default encoder2 value byte 1
	i2cdata[12]=0;									// default encoder2 value byte 2
	i2cdata[13]=0;									// default encoder2 value byte 3
	i2cdata[14]=0;									// default encoder2 value byte 4
}

void setMD49data(void)
{
	//set acceleration if changed since last set of acceleration
	if (i2cdata[2] != recentAcceleration) {setAcceleration(i2cdata[2]);}

	//set mode if changed since last set of Mode
	if (i2cdata[3] != recentMode) {setMode(i2cdata[3]);recentMode=i2cdata[3];}

	//set speed continuously, when timeout is enabled
	if (statusTimeout==1){
		setSpeed1(i2cdata[0]);
		setSpeed2(i2cdata[1]);
	}
	//set speed once changed, when timeout is disabled
	if (statusTimeout==0){
		if (recentSpeed1 != i2cdata[0]) {setSpeed1(i2cdata[0]);}
		if (recentSpeed2 != i2cdata[1]) {setSpeed2(i2cdata[1]);}
	}

	//reset encoders if byte was checked
	if (i2cdata[4]==1){
		resetEncoders();
		i2cdata[4]=0;
	}

	//set regulator if changed
	if (i2cdata[5]==0){
		if (statusRegulator != i2cdata[5]) {disableRegulator();}
	}
	else if (i2cdata[5]==1){
		if (statusRegulator != i2cdata[5]) {enableRegulator();}
	}

	// set timeout if changed
	if (i2cdata[6]==0){
		if (statusTimeout != i2cdata[6]) {disableTimeout();}
	}
	else if (i2cdata[6]==1){
		if (statusTimeout != i2cdata[6]) {enableTimeout();}
	}
}
