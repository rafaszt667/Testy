﻿/*
 * ADC.h
 *
 * Created: 2016-04-29 14:53:26
 *  Author: Rafał
 */ 


#ifndef ADC_H_
#define ADC_H_

typedef enum ADC_name
{
	A,
	B
}ADC_name_t;
	
typedef enum ADC_prescaler
{
		
};
void ADC_init(ADC_name_t ADCx, ADC_PRESCALER_t prescaler, ADC_REFSEL_t refference, ADC_conv_mode_t mode, freerun_t free_run, ADC_SWEEP_t sweep);



#endif /* ADC_H_ */