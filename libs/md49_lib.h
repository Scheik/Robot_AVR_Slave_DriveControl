/*
 * md49_lib.h
 *
 *  Created on: 22.09.2014
 *      Author: Scheik
 */

#ifndef MD49_LIB_H_
#define MD49_LIB_H_

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


#endif /* MD49_LIB_H_ */
