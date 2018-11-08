/*
 * Relee.h
 *
 * Created: 8/1/2018 11:40:09 AM
 *  Author: VladBaciu
 */ 


#ifndef RELEE_H_
#define RELEE_H_

#define FED_PUMP 0
#define DISCHARGE_PUMP 1
#define AIR_PUMP 2
#define AIR_PUMP_2 3

void relay_init();
void relay_control(unsigned char comand);




#endif /* RELEE_H_ */