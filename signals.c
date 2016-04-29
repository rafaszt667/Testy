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
#define t 1000000/sampling_period_us //sampling period in seconds

uint16_t samples(uint16_t freq_mHz);

uint16_t calc_sin(uint16_t freq_mHz, uint16_t max, uint16_t min)
{
	uint16_t N=0;		//co jaki czas pobierana probka
	uint16_t DC=0;		//wzmocnienie dc
	uint16_t A=0;		//amplituda
	
	
	A = (max - min)/2;
	
	DC = max - A;
	
	N=1000;
	
	for(uint8_t i=0; i < N; i++)
	{
		signal[i] = A * sin(2*3.14*0.01*i) + DC;
	}
	
	return N;
	
	/*
	sinus[i] = A * sin(2*PI*(freq_mHz/1000)*t*i) // sin=(2*pi*f*t) 
	*/
	
}

uint16_t calc_triangle(uint16_t freq_mHz, uint16_t max, uint16_t min)
{
	uint16_t N = 0;
	uint16_t DC = 0;
	uint16_t A = 0;
	
	A = (max - min)/2;	
	DC = max - A;
	N = samples(freq_mHz); 
	
	for(uint8_t i=0; i<N ; i++)
	{
		if ((t*i) < (t* N/2))
			signal[i] = 2*A/(t* (N/2 -1)) * t*i + (DC-A);
		else
			signal[i] = -2*A/(t* (N/2 -1)) * t*i + (DC-3*A);
	}
}

uint16_t calc_sowtooth(uint16_t freq_mHz, uint16_t max, uint16_t min)
{
	
}
uint16_t samples(uint16_t freq_mHz)
{
	return 1000000/(freq_mHz*t);
}