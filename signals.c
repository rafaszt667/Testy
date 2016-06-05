/*
 * signals.c
 *
 * Created: 2016-04-07 22:31:34
 *  Author: Rafał
 */ 
#include <avr/io.h>
#include <math.h>
#include "signals.h"

#define PI 3.14
#define PI2	6.28
//#define t sampling_period_us/1000000 //sampling period in seconds




void signal_param(uint32_t freq_mHz, uint16_t max_mV, uint16_t min_mV)
{
	signal_setings.wt = 2*PI*(float)freq_mHz/1000 * 0.001;
	signal_setings.N = (1/((float)freq_mHz/1000)) * 1000;
	signal_setings.A = (max_mV - min_mV)/2;
	signal_setings.DC = max_mV - signal_setings.A;
	
}

uint16_t sinus_sample(uint16_t sample)
{
	
return signal_setings.A * sin(signal_setings.wt * sample) + signal_setings.DC;


}

uint16_t triangle_sample(uint16_t sample)
{
	float t = 0.001;
	
	if ((t*sample) <= (t* signal_setings.N/2 ))
		return (2*signal_setings.A/(t*(signal_setings.N/2 - 1)) * t*sample) + (signal_setings.DC-signal_setings.A);
		
	else 
		return (-2*(int32_t)signal_setings.A/(t* (signal_setings.N/2 -1)) * t*sample) + (signal_setings.DC + (3*signal_setings.A));
		
		

	/*uint16_t N = 0;
	uint16_t DC = 0;
	uint16_t A = 0;
	
	A = (max - min)/2;	
	DC = max - A;
	N = samples(freq_mHz); 
	
	for(uint8_t i=0; i<N ; i++)
	{
		if ((t*i) < (t* N/2))
		//	signal[i] = 2*A/(t* (N/2 -1)) * t*i + (DC-A);
		//else
			//signal[i] = -2*A/(t* (N/2 -1)) * t*i + (DC-3*A);
	}*/

}

uint16_t sowtooth_sample(uint16_t sample)
{
	float t = 0.001;
		
	return 2*signal_setings.A/(t*(signal_setings.N-1)) * t*(sample-1) + (signal_setings.DC-signal_setings.A);
}
uint16_t samples(uint16_t freq_mHz)
{
	return 0;//1000000/(freq_mHz*t);
}