/*
 * signals.h
 *
 * Created: 2016-04-07 22:31:44
 *  Author: Rafał
 */ 


#ifndef SIGNALS_H_
#define SIGNALS_H_

#define sampling_period_us 100

uint8_t signal[1000];

uint16_t calc_sin(uint16_t freq_mHz, uint16_t max, uint16_t min);
uint16_t calc_triangle(uint16_t freq_mHz, uint16_t max, uint16_t min);
uint16_t calc_sowtooth(uint16_t freq_mHz, uint16_t max, uint16_t min);

#endif /* SIGNALS_H_ */