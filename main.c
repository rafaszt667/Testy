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

	ADC_init(A, ADC_PRESCALER_DIV512_gc, ADC_REFSEL_INT1V_gc, signed_mode, enable_freerun, ADC_SWEEP_01_gc );
	ADC_CH_init(A, CH0, ADC_CH_INPUTMODE_DIFF_gc, ADC_CH_MUXPOS_PIN5_gc, ADC_CH_MUXNEG_PIN1_gc, 0);
	ADC_CH_init(A, CH1, ADC_CH_INPUTMODE_DIFF_gc, ADC_CH_MUXPOS_PIN6_gc, ADC_CH_MUXNEG_PIN2_gc, 0);
	ADC_enable(A);
	
	
	DAC_init(DAC_CHSEL_DUAL_gc, DAC_REFSEL_INT1V_gc);
	
	lcd_init();

	N = calc_sin(100, 900, 0);

	DIROUT(led);
	DIRIN(sw0);
	PULLUP(sw0);

	uint16_t actual;
	

	
	int16_t reg;
	
	actual = ADCA_result_mV(CH0);
	
	PID(200,actual);
		uint16_t i =0 ;
    while (1) 
    {

		
		if(flag)
		{
			DAC_CH0_mV(signal[i]);
			
			if(++i > 999)
				i=0;
			/*actual = ADCA_result_mV(CH0);
			reg = PID(200,actual);
			if(reg>1000)
				reg = 1000;
				
			DAC_CH0_mV(reg);
			
		if(++i==100)
		{
			i=0;
		
		
		
		
		lcd_locate(0,0);
		lcd_str("ADC: ");
		lcd_int(actual);
		lcd_str("mV ");
		
		lcd_locate(1,0);
		lcd_str("DAC: ");
		lcd_int(reg);
		lcd_str("mV ");
		}*/
		flag =0;
		}
	
    }
}


ISR(TCC0_CCA_vect)
{
	
	
	TCC0.CCA += 50;
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
	TCC0.CCA =	50;
	TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
	
}



