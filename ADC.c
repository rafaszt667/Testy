/*
 * ADC.c
 *
 * Created: 2016-03-27 15:06:47
 *  Author: Rafał
 */ 
#include <avr/io.h>
#include "ADC.h"
#include "NVM.h"




uint16_t ADCA_refsource_mV, ADCB_refsource_mV;

uint16_t ADCA_resolution=4096 , ADCB_resolution = 4096;
uint32_t deltamV_A=0, deltamV_B=0;

float ADCA_full_factor_CH[4];
float ADCB_full_factor_CH[4];

float ADCA_gain_error_CH[4] ={1,1,1,1};
float ADCB_gain_error_CH[4] ={1,1,1,1};

uint8_t ADCA_offset_CH[4] ={0,0,0,0};
uint8_t ADCB_offset_CH[4] ={0,0,0,0};

void ADC_init(ADC_name_t ADCx, ADC_prescaler_t prescaler, ADC_refference_t refference, ADC_conv_mode_t mode, ADC_freerun_t freerun, ADC_sweep_t sweep)
{		
	
	if(ADCx == A)
	{	
		ADCA.PRESCALER = prescaler;						//ADC prescaler
		
		
		if(refference == ADC_REFSEL_INT1V_gc)			//if refference = 1V 
		{	
			ADCA.REFCTRL =	ADC_REFSEL_INT1V_gc |		//ADC reference
			ADC_CH_MUXINT_BANDGAP_gc;					//Enable bandgap
			
			ADCA_refsource_mV = 1000;					//refference source voltage in mV used to convert bit result to mV
		}
		else 
		{
			ADCA.REFCTRL = refference;
			ADCA_refsource_mV = ADCA_ref_source_val_mV;
		}
				
		if(freerun == ADC_FREERUN_ENABLE) ADCA.CTRLB = ADC_FREERUN_bm;	//Freerun mode
		
				
		if(mode == ADC_MODE_SIGNED)						//if signed mode
		{
			ADCA.CTRLB |= ADC_CONMODE_bm;				//signed conversion mode
			ADCA_resolution = 2048;						//change resolution 	 
		}
		else 
			deltamV_A = ADCA_refsource_mV*50;			//to unsigned mode use delta V
			
		ADCA.EVCTRL = sweep;							//Freerun trigger for declareted channels
		
		ADCA.CALL = ReadCalibrationByte(PRODSIGNATURES_ADCACAL0);	//Read production callibration row
		ADCA.CALH = ReadCalibrationByte(PRODSIGNATURES_ADCACAL1);
		
	}
	else
	{	
		ADCB.PRESCALER = prescaler;						//ADC prescaler
			
			
		if(refference == ADC_REFSEL_INT1V_gc)			//if refference = 1V
		{
			ADCB.REFCTRL =	ADC_REFSEL_INT1V_gc |		//ADC reference
			ADC_CH_MUXINT_BANDGAP_gc;					//Enable bandgap
				
			ADCA_refsource_mV = 1000;					//refference source voltage in mV used to convert bit result to mV
		}
		else
		{
			ADCB.REFCTRL = refference;
			ADCB_refsource_mV = ADCB_ref_source_val_mV;
		}
			
		if(freerun == ADC_FREERUN_ENABLE) ADCA.CTRLB = ADC_FREERUN_bm;	//Freerun mode
			
			
		if(mode == ADC_MODE_SIGNED)									//if signed mode
		{
			ADCA.CTRLB |= ADC_CONMODE_bm;				//signed conversion mode
			ADCA_resolution = 2048;						//change resolution
		}
		else
		deltamV_A = ADCA_refsource_mV*50;			//to unsigned mode use delta V
			
		ADCB.EVCTRL = sweep;							//Freerun trigger for declareted channels
			
		ADCB.CALL = ReadCalibrationByte(PRODSIGNATURES_ADCBCAL0);	//Read production callibration row
		ADCB.CALH = ReadCalibrationByte(PRODSIGNATURES_ADCBCAL1);	
	}
}

void ADC_CH_init(ADC_name_t ADCx, ADC_channel_t CHx,  ADC_CH_INPUTMODE_t inputmode, ADC_CH_MUXPOS_t possitive_pin, ADC_CH_MUXNEG_t negative_pin, ADC_CH_GAIN_t gain)
{
	float ADC_gain=1;
	ADC_CH_t * ADCx_CHx;
	
	if(inputmode == ADC_CH_INPUTMODE_DIFFWGAIN_gc)
		switch(gain)
		{
			case ADC_CH_GAIN_2X_gc:
				ADC_gain = 2;
				break;
			case ADC_CH_GAIN_4X_gc:
				ADC_gain = 4;
				break;
			case ADC_CH_GAIN_8X_gc:
				ADC_gain = 8;
				break;
			case ADC_CH_GAIN_16X_gc:
				ADC_gain = 16;
				break;
			case ADC_CH_GAIN_32X_gc:
				ADC_gain = 32;
				break;
			case ADC_CH_GAIN_64X_gc:
				ADC_gain = 64;
				break;
			case ADC_CH_GAIN_DIV2_gc:
				ADC_gain = 0.5;
				break;
			default:
				ADC_gain = 1;
				break;
		}
					
	if(ADCx == A)
	{
		switch(CHx)
		{
			case CH0:
				ADCx_CHx = &ADCA.CH0;
				break;
			case CH1:
				ADCx_CHx = &ADCA.CH1;
				break;
			case CH2:
				ADCx_CHx = &ADCA.CH2;
				break;
			case CH3:
				ADCx_CHx = &ADCA.CH3;
				break;	
			default:
				return;		
		}
			ADCA_full_factor_CH[CHx] = (float)ADCA_refsource_mV/(ADCA_resolution*ADC_gain);
	}
	else 
	{
		switch(CHx)
		{
			case CH0:
				ADCx_CHx = &ADCB.CH0;
				break;
			case CH1:
				ADCx_CHx = &ADCB.CH1;
				break;
			case CH2:
				ADCx_CHx = &ADCB.CH2;
				break;
			case CH3:
				ADCx_CHx = &ADCB.CH3;
				break;
			default:
				return;
		}
		if(!ADC_gain)
			ADCB_full_factor_CH[CHx] = (ADCB_refsource_mV/ADCB_resolution) * 2;
		else
			ADCB_full_factor_CH[CHx] = ADCB_refsource_mV/(ADCB_resolution*ADC_gain);
	}
	
	ADCx_CHx->CTRL = inputmode;							//diff mode
	ADCx_CHx->MUXCTRL =	possitive_pin | negative_pin ;	//Positive and negative input pin
}

int16_t ADCA_result_mV(ADC_channel_t CHx)
{
	int16_t result=0;
	
	switch(CHx)
	{
		case CH0:
			result = ADCA.CH0RES;
			break;
		case CH1:
			result = ADCA.CH1RES;
			break;
		case CH2:
			result = ADCA.CH2RES;
			break;
		case CH3:
			result = ADCA.CH3RES;
			break;
	}
	
	return (((int32_t) result * ADCA_full_factor_CH[CHx]) - deltamV_A - ADCA_offset_CH[CHx]) * ADCA_gain_error_CH[CHx] ;
}

int16_t ADCB_result_mV(ADC_channel_t CHx)
{
	int16_t result=0;
	
	switch(CHx)
	{
		case CH0:
			result = ADCB.CH0RES;
			break;
		case CH1:
			result = ADCB.CH1RES;
			break;
		case CH2:
			result = ADCB.CH2RES;
			break;
		case CH3:
			result = ADCB.CH3RES;
			break;
	}
	
	return (((int32_t) result * ADCB_full_factor_CH[CHx]) - deltamV_B - ADCB_offset_CH[CHx]) * ADCB_gain_error_CH[CHx] ;
}
void ADC_enable(ADC_name_t ADCx)
{
	if(ADCx == A)	ADCA.CTRLA = ADC_ENABLE_bm;
	else			ADCB.CTRLA = ADC_ENABLE_bm;
}

void ADC_disable(ADC_name_t ADCx)
{
	if(ADCx == A)	ADCA.CTRLA &= ~ADC_ENABLE_bm;
	else			ADCB.CTRLA &= ~ADC_ENABLE_bm;
}

void ADC_set_offset(ADC_name_t ADCx, ADC_channel_t CHx, uint8_t offset_mv)
{
	if(ADCx == A)
		ADCA_offset_CH[CHx] = offset_mv;
	else
		ADCB_offset_CH[CHx] = offset_mv;
}
void ADC_set_gain_error(ADC_name_t ADCx, ADC_channel_t CHx, float gainerr_factor)
{
	if(ADCx == A)
		ADCA_gain_error_CH[CHx] = gainerr_factor;
	else 
		ADCB_gain_error_CH[CHx] = gainerr_factor;
}
void ADC_auto_offset(ADC_name_t ADCx, ADC_channel_t CHx)
{
	int16_t result =0;
	
	if(ADCx == A)
	{
		switch(CHx)
		{
			case CH0:
			result = ADCA.CH0RES;
			break;
			case CH1:
			result = ADCA.CH1RES;
			break;
			case CH2:
			result = ADCA.CH2RES;
			break;
			case CH3:
			result = ADCA.CH3RES;
			break;
		}
		ADCA_offset_CH[CHx] = ((int32_t)result * ADCA_full_factor_CH[CHx]) -deltamV_A;
	}
	else
	{
		switch(CHx)
		{
			case CH0:
			result = ADCB.CH0RES;
			break;
			case CH1:
			result = ADCB.CH1RES;
			break;
			case CH2:
			result = ADCB.CH2RES;
			break;
			case CH3:
			result = ADCB.CH3RES;
			break;
		}
		ADCB_offset_CH[CHx] = ((int32_t)result * ADCB_full_factor_CH[CHx]) -deltamV_B;
	}
}

