/*
 * Testy.c
 *
 * Created: 2016-03-22 19:23:38
 * Author : Rafał
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Makros_and_uC_config.h"
#include "lcd44780.h"
#include "ADC.h"
#include "DAC.h"
#include "PID.h"
#include "signals.h"

#define led led
#define ledpin 0
#define ledport D

#define sw0 sw0
#define sw0pin 1
#define sw0port D

#define sw1 sw1
#define sw1pin 2
#define sw1port D



void OSC_init(void);
void TC0_init(void);


volatile uint8_t flag=0;
int16_t adc_result=0;
uint16_t N;

int main(void)
{
	
	OSC_init();
	TC0_init();

	ADC_init(A,ADC_DIV512,ADC_REF_INT1V,ADC_MODE_SIGNED,ADC_FREERUN_ENABLE,ADC_SWEEP_CH01);
	ADC_CH_init(A,ADC_CH0, ADC_INPUTMODE_DIFF,ADC_POSPIN5,ADC_NEGPIN1,0);
	ADC_CH_init(A,ADC_CH1, ADC_INPUTMODE_DIFF,ADC_POSPIN6,ADC_NEGPIN2,0);	
	ADC_enable(A);
	
	DAC_init(DAC_CHDUAL,DAC_REF_INT1V);
	
	lcd_init();


	DIROUT(led);
	DIRIN(sw0);
	PULLUP(sw0);

	uint16_t actual=300;
	

	
	
//	actual = ADCA_result_mV(CH0);
	
	//PID(200,actual);
		uint16_t i =0 ;
    while (1) 
    {
		lcd_locate(0,0);
		lcd_int(actual);
		
		if(flag)
		{
			actual = ADCA_result_mV(ADC_CH0);
			DAC_CH0_mV(500);
			
	//		DAC_CH0_mV(signal[i]);
			
			if(++i > 999)
				i=0;
		flag =0;
		}
	
    }
}


ISR(TCC0_CCA_vect)
{
	
	
	TCC0.CCA += 31250/5;
	flag=1;
	TGL(led);
	
}



void OSC_init(void)
{
	OSC.CTRL = OSC_RC32MEN_bm;
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	
	CPU_CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

void TC0_init(void)
{
	TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;
	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm;
	
	sei();
	
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCC0.CTRLFCLR = TC0_DIR_bm;
	TCC0.CCA =	31250/5;
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;
	
}



