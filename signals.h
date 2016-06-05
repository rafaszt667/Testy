/*
 * signals.h
 *
 * Created: 2016-04-07 22:31:44
 *  Author: Rafał
 */ 


#ifndef SIGNALS_H_
#define SIGNALS_H_

#define sampling_period_us 1000

struct
{
	float wt;
	uint16_t N;
	uint16_t DC;
	uint16_t A;
	uint32_t fn;
} signal_setings;

void signal_param(uint32_t freq_mHz, uint16_t max_mV, uint16_t min_mV);
uint16_t sinus_sample(uint16_t sample);
uint16_t triangle_sample(uint16_t sample);
uint16_t sowtooth_sample(uint16_t sample);

#endif /* SIGNALS_H_ */