/*
 * Sensor.h
 *
 * Created: 2016-09-10 20:49:46
 *  Author: Rafał
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

//Ustawnia dla grzejnika:
#define HEATER_V_ADC ADC_A		//przetwornik do pomiaru pradu na grzejniku
#define HEATER_V_ADC_CH ADC_CH0	//kanał przetwornika do pomiaru napięcia na grzejniku
#define SENSOR_R_ADC_CH ADC_CH2 //kanal przetwornika do pomiaru napiecia do obliczenia rezsytancji sensora
#define SENSOR_VOLTAGE_mV 3150


#define HEATER_I_ADC ADC_B		//przetwornik do pomiaru napiecia na grzejniku
#define HEATER_I_ADC_CH ADC_CH1	//kanał przetwornika do poomiaru prądu
#define CURRENT_R_mOHM	2000	//rezystancja rezystora do pomiaru pradu w mili omach

//Ustawienia dla rezystancji sensora:

//Podłączenie rezystorów:
//100 ohm
#define sensor_R100 sensor_R100
#define sensor_R100pin	0
#define sensor_R100port C

//1k ohm
#define sensor_R1k sensor_R1k
#define sensor_R1kpin	1
#define sensor_R1kport	C

//10k ohm
#define sensor_R10k sensor_R10k
#define sensor_R10kpin	2
#define sensor_R10kport C

//100k ohm
#define sensor_R100k sensor_R100k
#define sensor_R100kpin	3
#define sensor_R100kport C




void sensor_init(void);	//inicjalizacja peryferii dla obslugi czujnika

uint16_t heater_voltage_mV(void);
uint16_t heater_current_mA(void);
uint16_t heater_temperature(uint16_t heater_U_mV, uint16_t heater_I_mA);

uint16_t sensor_res_ohm(void);

#endif /* SENSOR_H_ */