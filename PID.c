/*
 * PID.c
 *
 * Created: 2016-03-28 18:03:23
 *  Author: Rafał
 */ 
#include <avr/io.h>
#include "PID.h"

#define INT_MAX	5000

int32_t integral(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV);
int16_t derivative(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV);



int16_t PID(uint16_t set_point, uint16_t actual_value, uint16_t f_sample_Hz)
{
	static int16_t last_e = 0;	//poprzedni uchyb
	static int32_t C = 0;		//wartość całki
	int16_t now_e;				//aktualny uchyb
	int32_t R;				//wartość różniczki
	int32_t new_C;			//wartość całki pomiędzy uchybi (aktualnie wyliczonej)
	int16_t signal;			//aktualna wartość na wyjściu regulatora
	uint8_t dt_ms;

	dt_ms = 1000 / (float)f_sample_Hz;	//okres próbkowania w ms
	now_e = set_point - actual_value;	//obliczenie aktualnego uchybu

	R = derivative(dt_ms, last_e, now_e);		//różniczka	
	new_C = integral(dt_ms, last_e, now_e);	//aktualna wartość całki (pomiedzy uchybami)

	C+= new_C;			//dodanie aktualnej wartości do całkowitej wartości całki
	if(C < -INT_MAX )			//ograniczenie całki od góry i od dołu
			C = -INT_MAX;
			
	else if(C> INT_MAX )		
			C = INT_MAX;
	
	signal =  ( Kp *(now_e +  ((1/Ti) * C/1000 )) + (Td * R * 1000) );		//Obliczenie wyjścia regulatora
	
	if (signal < 0)		//ograniczenie wyjścia regulatora od dołu
		signal = 0;
		
	last_e = now_e;	//nowy uchyba = stary uchyb
	
	return signal;
}
int16_t derivative(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV)
{
	return (last_e_mV - now_e_mV) / dt_ms;
}

int32_t integral(uint8_t dt_ms, int16_t last_e_mV, int16_t now_e_mV)
{
	return ( (last_e_mV + now_e_mV) / 2) * dt_ms;
}