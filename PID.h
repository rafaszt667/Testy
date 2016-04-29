/*
 * PID.h
 *
 * Created: 2016-03-28 18:03:34
 *  Author: Rafał
 */ 


#ifndef PID_H_
#define PID_H_

#define sampling_period_ms 1
#define Kp 0.3

#define Ti 0.004
#define Td 0.0001

int16_t PID(uint16_t set_point, uint16_t actual_value);

#endif /* PID_H_ */