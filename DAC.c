/*
 * DAC.c
 *
 * Created: 2016-03-28 13:14:13
 *  Author: Rafał
 */ 
#include <avr/io.h>
#include "DAC.h"
#include "NVM.h"
uint16_t DAC_refsource_mV;


void DAC_init(DAC_CHSEL_t CH_SEL, DAC_REFSEL_t refference )
{
	DACB.CTRLC = refference;
	
	if(refference == DAC_REFSEL_INT1V_gc)
		DAC_refsource_mV = 1000;
	else 
		DAC_refsource_mV = DAC_ref_source_val_mV;

	
	switch(CH_SEL)
	{
		case DAC_CHSEL_SINGLE_gc:
			DACB.CTRLB = CH_SEL ;
			DACB.CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;
			break;
		case DAC_CHSEL_SINGLE1_gc:
			DACB.CTRLB = CH_SEL ;
			DACB.CTRLA = DAC_CH1EN_bm | DAC_ENABLE_bm;
			break;
		case DAC_CHSEL_DUAL_gc:
			DACB.CTRLB = CH_SEL ;
			DACB.CTRLA = DAC_CH0EN_bm | DAC_CH1EN_bm | DAC_ENABLE_bm;
			break;			
		default:
			break;
	}
	
	DACB.CH0OFFSETCAL=ReadCalibrationByte(PRODSIGNATURES_DACB0OFFCAL);
	DACB.CH0GAINCAL=ReadCalibrationByte(PRODSIGNATURES_DACB0GAINCAL);
	DACB.CH1OFFSETCAL=ReadCalibrationByte(PRODSIGNATURES_DACB1OFFCAL);
	DACB.CH1GAINCAL=ReadCalibrationByte(PRODSIGNATURES_DACB1GAINCAL);				
}

void DAC_CH0_mV(uint16_t mV)
{

	while(!(DACB.STATUS & DAC_CH0DRE_bm));
	
	DACB.CH0DATA =  ( (uint32_t)mV * 0x000FFF)/(DAC_refsource_mV );
}

void DAC_CH1_mV(uint16_t mV)
{
	while(!(DACB.STATUS & DAC_CH1DRE_bm));
	
	
	DACB.CH1DATA = ( (uint32_t)mV * 0xFFF)/DAC_refsource_mV;
}

