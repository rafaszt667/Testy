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

enum {sinus, trian, sowto};
float parameter[3];	//parame

uint16_t Ampl;	//amplituda przebiegu
uint16_t DC;	//składowa stała przebiegu
uint32_t N;		//ilosc próbek na okres
float t;		//okres próbkowania

uint32_t signal_param(uint32_t freq_mHz, uint16_t max_mV, uint16_t min_mV, uint16_t f_sample_Hz)
{
	N	= ((uint32_t)f_sample_Hz * 1000)/freq_mHz;
	t	= 1/(float)f_sample_Hz;
	Ampl= (max_mV - min_mV) / 2;	//amplituda przebiegu
	DC	= min_mV + Ampl;			//stała DC przebiegu
	
//parametry do przebiegów (w celu optymalizacji wyliczane wczesniej)
	parameter[sinus] = 2*PI*((float)freq_mHz/1000) * (1/(float)f_sample_Hz);  //omega t dla sinusa
	parameter[trian] = 2*(float)Ampl/( t * (N/2 - 1) ) * t ; //parametr do przebiegu trójkątnego, współczynnik a funkcji liniowej do tworzenia przebiegu
	parameter[sowto] = 2*(float)Ampl/( t * (N-1) ) * t;		//parametr do przebiegu piłokształtnego, współczynnik a funkcji lniowej
	
	return N;
}

uint16_t sinus_sample(uint32_t sample)
{	
return Ampl * sin(parameter[sinus] * sample) + DC;
}

uint16_t triangle_sample(uint16_t sample)
{
	if ((t*sample) <= (t* N/2 ))
		return parameter[trian] * sample + (DC - Ampl);	//zbocze narazstajace przebiegu trojkatnego (rosnaca funkcja liniowa)	
	else 
		return -parameter[trian] * sample + (DC + (3*Ampl));//zbocze opadajace przebiegu trojkatnego (malejaca funkcja liniowa)
}

uint16_t sowtooth_sample(uint16_t sample)
{		
	return parameter[sowto] * (sample-1) + (DC-Ampl);
}
