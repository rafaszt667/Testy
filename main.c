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

#define div 1000
#define divcca 32000000/64/div

void OSC_init(void);
void TC0_init(void);


volatile uint8_t flag=0;
int16_t adc_result=0;
uint16_t N;
		uint16_t i = 0;
//uint16_t div;
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
	
	signal_param(10000,900,100);
	
	//div = (32000000/64)/signal_setings.fn; //obliczanie dzielnika dodatkowego dla timera aby uzyskać odpowiednią czestotliwość probkowania w zależnosci od częstotliwości sygnału i liczby próbek.
//	actual = ADCA_result_mV(CH0);
	
	//PID(200,actual);

		
    while (1) 
    {

			

		
		if(flag)
		{		



			
		flag =0;
		}
	
    }
}


ISR(TCC0_CCA_vect)
{
	
	
	TCC0.CCA += divcca;
	
	DAC_CH0_mV(sowtooth_sample(i));
	
	++i;
	if(i == signal_setings.N)
		i=0;
	
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
	TCC0.CCA =	divcca;
	TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
	
}



