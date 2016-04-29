/*
 * PID.c
 *
 * Created: 2016-03-28 18:03:23
 *  Author: Rafał
 */ 
#include <avr/io.h>
#include "PID.h"


int32_t integral(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV);

int16_t derivative(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV);



int16_t PID(uint16_t set_point, uint16_t actual_value)
{
	static int16_t last_e=0;
	static int32_t C=0;
	
	int16_t now_e;
	int32_t R,new_C, signal;
	

	now_e = set_point - actual_value;
	
	new_C = integral(sampling_period_ms, last_e, now_e);

	C+= new_C;
	if((C<-50000) )
	{
			C=-50000;
	}
	else if((C>50000) )
	{
		
			C=50000;
	}

	
	R = derivative(sampling_period_ms, last_e, now_e);

	signal =  ( Kp *(now_e +  ((1/Ti) * C/1000 )) + (Td * R *1000) );
	if (signal < 0)
		signal =0;
		
	last_e = now_e;
	return signal;
	
}
int16_t derivative(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV)
{
	return (last_e_mV - now_e_mV) / dt_ms;
}

int32_t integral(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV)
{
	return ((last_e_mV + now_e_mV)/2)*dt_ms;
}