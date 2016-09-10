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
#include <math.h>

#include "Makros_and_uC_config.h"
#include "lcd44780.h"
#include "ADC.h"
#include "DAC.h"
#include "PID.h"
#include "signals.h"
#include "sensor.h"

#define led led
#define ledpin 0
#define ledport D

#define sw0 sw0
#define sw0pin 1
#define sw0port D

#define sw1 sw1
#define sw1pin 2
#define sw1port D

//częstotliwość próbkowania
#define F_SAMPLE 1000

#define divcca 32000000/64/F_SAMPLE //dzielnik dla timera (rejestru porównania cca)



void OSC_init(void);
void TC0_init(void);


volatile uint8_t flag=0;
uint32_t i = 0;
uint32_t N = 0;

uint16_t U = 0;
uint16_t I = 0;

uint32_t x = 0;
int32_t a = 0;

int main(void)
{
	
	OSC_init();
	TC0_init();

	lcd_init();

	DAC_init(DAC_CHDUAL,DAC_REF_INT1V);
		

	DIROUT(led);
	DIRIN(sw0);
	PULLUP(sw0);

	N = signal_param(50,400,300,F_SAMPLE);
	
	
		uint16_t R, W;		
		
    while (1) 
    {

		if(flag)
		{		
						
			U = heater_voltage_mV();
			I = heater_current_mA();
			a = heater_temperature(U*3/2, I);
			if((int16_t)a < 0 ) a=0;	
			W = sinus_sample(i);		
			R=PID(W,a,F_SAMPLE);
			if (R<300) R=300;
			DAC_CH0_mV(R);
			DAC_CH1_mV(a);

			++i;
			if(i == N)	//jesli aktualny numer pobieranej probki jest większy od N-1 to nastęnpna probka 0
			{
				i=0;
			}
			
		flag =0;
		}
    }
}


//Przerwanie od timer 0 kanał A
ISR(TCC0_CCA_vect)
{	
	TCC0.CCA += divcca;	//nastepne przerwanie bedzie gdy timer doliczy do wartosci wiekszej o kolejny dzielnik
	flag = 1;
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
	
	TCC0.CCA =	divcca;	//wartosc rejestru porownania (jesli stan rimera jak cca to przerwanie)
	TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
	
}



