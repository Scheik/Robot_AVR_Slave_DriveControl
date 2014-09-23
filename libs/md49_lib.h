/*
 * md49_lib.h
 *
 *  Created on: 22.09.2014
 *      Author: Scheik
 */

#ifndef MD49_LIB_H_
#define MD49_LIB_H_

// functions & procedures delaration
extern void setMode(char mode);
extern void resetEncoders(void);
extern void readEncoderValues(void);
extern void getSpeed1(void);
extern void getSpeed2(void);
extern void getVolts(void);
extern void getCurrent1(void);
extern void getCurrent1(void);
extern void getCurrent2(void);
extern void getAcceleration(void);
extern void getMode(void);
extern void getError(void);
extern void setSpeed1(char Speed1);
extern void setSpeed2(char Speed2);
extern void setAcceleration(char Acceleration);
extern void disableRegulator(void);
extern void enableRegulator(void);
extern void disableTimeout(void);
extern void enableTimeout(void);
extern void init_MD49data(void);
extern void setMD49commands(void);
extern void getMD49data (void);

// variables declaration
extern volatile char statusRegulator;						// stores state of regulator 0=off 1=on
extern volatile char statusTimeout;						// stores state of timeout 0=off 1=on
extern volatile char currentMode;							// stores mode recently set
extern volatile char currentAcceleration;					// stores acceleration recently set
extern volatile char recentSpeed1;							// stores recent set Speed1
extern volatile char recentSpeed2;							// stores recent set Speed2
//volatile uint8_t RXBuffer[8];
//volatile int32_t encoder1, encoder2;             	// To store the encoder values

#endif /* MD49_LIB_H_ */
