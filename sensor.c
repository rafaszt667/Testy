/*
 * sensor.c
 *
 * Created: 2016-09-10 20:49:58
 *  Author: Rafał
 */ 
#include <avr/io.h>
#include <math.h>
#include "ADC.h"
#include "sensor.h"
#include "Makros_and_uC_config.h"

//**********************************************************************************
//stale do obliczenia temperatury grzejnika:
#define SENSOR_A 0.0039083
#define SENSOR_B -0.0000005775

#define SENSOR_Rt_OHM 55
#define SENSOR_tA_stC 25
#define SENSOR_RH0 10//SENSOR_Rt_OHM / (1 + (SENSOR_A * SENSOR_tA_stC) + (SENSOR_B * SENSOR_tA_stC * SENSOR_tA_stC))

#define SENSOR_PREFIX1	SENSOR_A/(2*SENSOR_B)
#define SENSOR_PREFIX2	(SENSOR_A*SENSOR_A) / ( 4 * SENSOR_B*SENSOR_B)
#define SENSOR_DEN		( SENSOR_RH0 * SENSOR_B)
//************************************************************************************

typedef enum sensor_R
{
	R_100 = 100,
	R_1k  = 1000,
	R_10k = 10000,
	R_100k = 100000
}sensor_R_t;

sensor_R_t current_R;
void sensor_R_set(sensor_R_t R);

uint16_t sensor_voltage_mV(void);



void sensor_init(void)
{
	//Pomiar napiecia na grzejniku:
	ADC_init(HEATER_V_ADC, ADC_DIV512, ADC_REF_AREFB, ADC_MODE_SIGNED, ADC_FREERUN_ENABLE, ADC_SWEEP_CH01);
	ADC_CH_init(HEATER_V_ADC, HEATER_V_ADC_CH, ADC_INPUTMODE_DIFFWGAIN, ADC_POSPIN6, ADC_NEGPIN5, ADC_GAIN_1X);
	ADC_CH_init(HEATER_V_ADC, SENSOR_R_ADC_CH, ADC_INPUTMODE_DIFFWGAIN, ADC_POSPIN7, ADC_NEGPIN4, ADC_GAIN_1X);	
	ADC_enable(HEATER_V_ADC);	
	//Pomiar pradu na grzjeniku:
	ADC_init(HEATER_I_ADC, ADC_DIV512, ADC_REF_INT1V, ADC_MODE_SIGNED, ADC_FREERUN_ENABLE, ADC_SWEEP_CH01);
	ADC_CH_init(HEATER_I_ADC, HEATER_I_ADC_CH, ADC_INPUTMODE_DIFFWGAIN, ADC_POSPIN6, ADC_NEGPIN5, ADC_GAIN_1X);
	ADC_enable(HEATER_I_ADC);
	
	//zadawanie napiecia grzejnika:


	//inicjalizacja pomiaru rezystancji grzejnika:
	DIROUT(sensor_R100);
	DIROUT(sensor_R1k);
	DIROUT(sensor_R10k);
	DIROUT(sensor_R100k);
	
	sensor_R_set(R_100k);
}


uint16_t heater_voltage_mV(void)
{
	if(HEATER_V_ADC == ADC_A)	return ADCA_result_mV(HEATER_V_ADC_CH);
	else 						return ADCB_result_mV(HEATER_V_ADC_CH);
}
uint16_t heater_current_mA(void)
{
	if(HEATER_V_ADC == ADC_A)	return (ADCA_result_mV(HEATER_I_ADC_CH) * 1000) / CURRENT_R_mOHM;
	else 						return (ADCB_result_mV(HEATER_I_ADC_CH) * 1000) / CURRENT_R_mOHM;
}

uint16_t heater_temperature(uint16_t heater_U_mV, uint16_t heater_I_mA)
{
	float R = (float)heater_U_mV / heater_I_mA;
	
	return (uint16_t)(-(SENSOR_PREFIX1 + sqrt( SENSOR_PREFIX2 - (SENSOR_RH0 - R) / SENSOR_DEN) ) );	
}

uint16_t sensor_res_ohm(void)
{
	uint16_t Ur_mV;
	
	do 
	{
		Ur_mV = sensor_voltage_mV();
		
		if(Ur_mV < 2400)		sensor_R_set(current_R -1);
		else if (Ur_mV > 3000)	sensor_R_set(current_R +1);
		
	} while ( (Ur_mV < 2400) || (Ur_mV > 3000) );
	
	return current_R / (SENSOR_VOLTAGE_mV/Ur_mV - 1);
}

uint16_t sensor_voltage_mV(void)
{
	if(HEATER_V_ADC == ADC_A)	return ADCA_result_mV(SENSOR_R_ADC_CH);
	else 						return ADCB_result_mV(SENSOR_R_ADC_CH);
}

void sensor_R_set(sensor_R_t R)
{
	CLR(sensor_R100);
	CLR(sensor_R1k);
	CLR(sensor_R10k);
	CLR(sensor_R100k);
	
	switch(R)
	{
		case R_100:
			SET(sensor_R100);
			break;
		case R_1k:
			SET(sensor_R1k);
			break;
		case R_10k:
			SET(sensor_R10k);
			break;
		case R_100k:
			SET(sensor_R100k);
			break;
		default:
			break;	
	}
	
	current_R = R;
}