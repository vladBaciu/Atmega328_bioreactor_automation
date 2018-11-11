/*
 * bmp180.c
 *
 * Created: 9/11/2018 1:00:49 PM
 *  Author: Baciu Vlad
 */ 

#ifndef BMP180_H_
#define BMP180_H_

void IOInit(void);
void i2c_error(void);
uint8_t getDataFromBMP180Register(uint8_t loc);
void calibrate_bmp180(void);
long getUcTemp(void);
short getTrueTemp(long UT);
unsigned long getUcPressure(void);
long getTruePressure(unsigned long UP);
long getPressure(void);
short getTemp(void);
long getAltitude(void);



#endif /* BMP180_H_ */
