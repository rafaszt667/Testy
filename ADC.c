/*
 * ADC.c
 *
 * Created: 2016-03-27 15:06:47
 *  Author: Rafał
 */ 
#include <avr/io.h>
#include "ADC.h"
#include "NVM.h"



uint16_t ADCx_refsource_mV[2];		///<Aktualne napięcie referencyjne A (0) - ADCA B (1) -ADCB 
uint16_t ADCx_resolution[2];		///<Aktualna rozdzielczość ADC, zależy od mode w funkcji ADC_init. A (0) - ADCA B (1) -ADCB 
uint32_t ADCx_delta_mV[2] = {0,0};	///<Aktualna deltaV (wzor w dokumentacji) zależy od rozdzielczości. A (0) - ADCA B (1) -ADCB 

float ADCx_CHx_full_factor[2][4];				///<Aktualny współczynnik obliczony ze wszystkich powyższych stałych. 
												///<Pierwszy wymiar - A lub B (wybór przetwornika), drugi wymiar - ADC_CH0 ... ADC_CH3 (kanał przetwornika.
float ADCx_CHx_gain_error[2][4] = {{1,1,1,1},	///<Aktualny błąd wzmocnienia.
								  {1,1,1,1}};
									  
uint8_t ADCx_CHx_offset[2][4] ={0,0,0,0};		///<Aktualny offset kanału. 

void ADC_init(ADC_name_t adc_name, ADC_prescaler_t prescaler, ADC_refference_t refference, ADC_conv_mode_t mode, ADC_freerun_t freerun, ADC_sweep_t sweep)
{		
	ADC_t * ADCx;	///< Wskaźnik na odpowiedni ADC
	
	if(adc_name == ADC_A)
		ADCx = &ADCA;
	else 
		ADCx = &ADCB;
		
		
	ADCx->PRESCALER = prescaler;		//ustawienie preskalera
	
	if(refference == ADC_REF_INT1V)		//ustawienie napięcia referencyjnego
	{
		ADCx->REFCTRL = ADC_REF_INT1V | 
						ADC_CH_MUXINT_BANDGAP_gc;	//ustaw napięcie referencyjnego 1V i włącz BANDGAP dla wewnętrznego źródła 1V
	}
	else 
		ADCx->REFCTRL = refference;
		
	ADCx_refsource_mV[ADC_A] = ADCA_ref_source_val_mV;	
	ADCx_refsource_mV[ADC_B] = ADCB_ref_source_val_mV;
	
	
	if(freerun == ADC_FREERUN_ENABLE)		//ustawienie trybu freerun
		ADCx->CTRLB |= ADC_FREERUN_bm;
	else
		ADCx->CTRLB &= ~ADC_FREERUN_bm;
		
		
	if(mode == ADC_MODE_SIGNED)				//ustawienie trybu pomiaru
	{
		ADCx->CTRLB |= ADC_CONMODE_bm;		
		ADCx_resolution[adc_name] = 2048;	//dla trybu signed rozdzielczosć wynosi 2048
		ADCx_delta_mV[adc_name] = 0;
	}
	else 
	{
		ADCx->CTRLB &= ~ADC_CONMODE_bm;
		ADCx_resolution[adc_name] = 4096;							//dla trybu unsigned rozdzielczosc 4096
		ADCx_delta_mV[adc_name] = ADCx_refsource_mV[adc_name]*50;	//delta dla unsigned wynosi 5% napięcia referencyjnego (dokumentacja)
	}
	
	ADCx->EVCTRL = sweep;					//włączenie triggera dla wybranych kanałów  ADC (dla freerun)
	
	ADCA.CALL = ReadCalibrationByte(PRODSIGNATURES_ADCACAL0);	//czytanie z flasha i zapisywanie do rejestru bajtów kalibracyjnych 
	ADCA.CALH = ReadCalibrationByte(PRODSIGNATURES_ADCACAL1);
	
	ADCB.CALL = ReadCalibrationByte(PRODSIGNATURES_ADCACAL0);	
	ADCB.CALH = ReadCalibrationByte(PRODSIGNATURES_ADCACAL1);
	
		
}

void ADC_CH_init(ADC_name_t adc_name, ADC_channel_t ch_number,  ADC_inputmode_t inputmode, ADC_possitive_pin_t possitive_pin, ADC_negative_pin_t negative_pin, ADC_gain_t gain)
{
	uint8_t ADC_gain = 1;	///<Wzmocnienie dla trybu różnicowe z wzmocnieniem
	ADC_t * ADCx;			///<Wskaźnik na aktualny ADC
	ADC_CH_t * ADCx_CHx;	///<Wskaźnik na aktualny kanał
	
	if(adc_name == ADC_A)	//Przypisanie adresu ADC
		ADCx = &ADCA;
	else 
		ADCx = &ADCB;
	
	switch(ch_number)	//Przypisanie adresu kanału
	{
		case ADC_CH0:
			ADCx_CHx = &ADCx->CH0;
			break;
		case ADC_CH1:
			ADCx_CHx = &ADCx->CH1;
			break;
		case ADC_CH2:
			ADCx_CHx = &ADCx->CH2;
			break;
		case ADC_CH3:
			ADCx_CHx = &ADCx->CH3;
			break;
	}

	if(inputmode == ADC_INPUTMODE_DIFFWGAIN)	//Ustawienie wzmocnienia dla kanału jeśli tryb różnicowy z wzmocnieniem
	{
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
				ADC_gain = 0;
				break;
			default:
				ADC_gain = 1;
				break;
		}
	}
	
	//Obliczenie współczynnika dla danego kanału przetwornika. 
	//jeśłi gain == 0 to znaczy że wynosi 0.5
	if(ADC_gain == 0)
		ADCx_CHx_full_factor[adc_name][ch_number] = ((float)ADCx_refsource_mV[adc_name]/ADCx_resolution[adc_name]) * 2;
	else
		ADCx_CHx_full_factor[adc_name][ch_number] = (float)ADCx_refsource_mV[adc_name]/(ADCx_resolution[adc_name]*ADC_gain);	
			
	ADCx_CHx->CTRL = inputmode;							//Ustawienie trybu pomiaru
	ADCx_CHx->MUXCTRL =	possitive_pin | negative_pin ;	//Ustawienie podłączonych pinów.

}

int16_t ADCA_result_mV(ADC_channel_t ch_number)
{
	int16_t result=0;
	
	switch(ch_number)
	{
		case ADC_CH0:
			result = ADCA.CH0RES;
			break;
		case ADC_CH1:
			result = ADCA.CH1RES;
			break;
		case ADC_CH2:
			result = ADCA.CH2RES;
			break;
		case ADC_CH3:
			result = ADCA.CH3RES;
			break;
	}

	return (((int32_t) result * ADCx_CHx_full_factor[ADC_A][ch_number]) - ADCx_delta_mV[ADC_A] - ADCx_CHx_offset[ADC_A][ch_number]) * ADCx_CHx_gain_error[ADC_A][ch_number] ;
}

int16_t ADCB_result_mV(ADC_channel_t ch_number)
{
	int16_t result=0;
	
	switch(ch_number)
	{
		case ADC_CH0:
			result = ADCB.CH0RES;
			break;
		case ADC_CH1:
			result = ADCB.CH1RES;
			break;
		case ADC_CH2:
			result = ADCB.CH2RES;
			break;
		case ADC_CH3:
			result = ADCB.CH3RES;
			break;
	}
	
	return (((int32_t) result * ADCx_CHx_full_factor[ADC_B][ch_number]) - ADCx_delta_mV[ADC_B] - ADCx_CHx_offset[ADC_B][ch_number]) * ADCx_CHx_gain_error[ADC_B][ch_number] ;
}
void ADC_enable(ADC_name_t adc_name)
{
	if(adc_name == ADC_A)	ADCA.CTRLA = ADC_ENABLE_bm;
	else			ADCB.CTRLA = ADC_ENABLE_bm;
}

void ADC_disable(ADC_name_t adc_name)
{
	if(adc_name == ADC_A)	ADCA.CTRLA &= ~ADC_ENABLE_bm;
	else			ADCB.CTRLA &= ~ADC_ENABLE_bm;
}

void ADC_set_offset(ADC_name_t adc_name, ADC_channel_t ch_number, uint8_t offset_mv)
{
	if(adc_name == ADC_A)
		ADCx_CHx_offset[ADC_A][ch_number] = offset_mv;
	else
		ADCx_CHx_offset[ADC_B][ch_number] = offset_mv;
}
void ADC_set_gain_error(ADC_name_t adc_name, ADC_channel_t ch_number, float gainerr_factor)
{
	if(adc_name == ADC_A)
		ADCx_CHx_gain_error[ADC_A][ch_number] = gainerr_factor;
	else 
		ADCx_CHx_gain_error[ADC_B][ch_number] = gainerr_factor;
}
void ADC_auto_offset(ADC_name_t ADC_name, ADC_channel_t ch_number)
{
	int16_t result = 0;
	ADC_t * ADCx;
	
	if (ADC_name == ADC_A)
		ADCx = &ADCA;
	else 
		ADCx = &ADCB;
		
	switch(ch_number)
	{
		case ADC_CH0:
		result = ADCx->CH0RES;
		break;
		case ADC_CH1:
		result = ADCx->CH1RES;
		break;
		case ADC_CH2:
		result = ADCx->CH2RES;
		break;
		case ADC_CH3:
		result = ADCx->CH3RES;
		break;
	}
	ADCx_CHx_offset[ADC_name][ch_number] = ((int32_t)result * ADCx_CHx_full_factor[ADC_name][ch_number]) - ADCx_delta_mV[ADC_name];
}

inline void ADC_set_full_factor(ADC_name_t adc_name, ADC_CH_t ch_number)
{
	return;
}