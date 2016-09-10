/*
 * PID.h
 *
 * Created: 2016-03-28 18:03:34
 *  Author: Rafał
 */ 


#ifndef PID_H_
#define PID_H_

#define Kp 10
#define Ti 0.00001
#define Td 0

int16_t PID(uint16_t set_point, uint16_t actual_value, uint16_t f_sample_Hz);

#endif /* PID_H_ */