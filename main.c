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
#define F_SAMPLE 10000

#define divcca 32000000/64/F_SAMPLE //dzielnik dla timera (rejestru porównania cca)


#define ADC_V_I_NAME ADC_A		//przetwornik do pomiarów
#define ADC_V_CH ADC_CH0	//kanał przetwornika do pomiaru napięcia
#define ADC_I_CH ADC_CH1	//kanał przetwornika do poomiaru prądu

#define CURRENT_R_mOHM	1000
#define SENSOR_A 0.0039083
#define SENSOR_B -0.0000005775

#define SENSOR_Rt_OHM 55
#define SENSOR_tA_stC 25
#define SENSOR_RH0 SENSOR_Rt_OHM / (1 + (SENSOR_A * SENSOR_tA_stC) + (SENSOR_B * SENSOR_tA_stC * SENSOR_tA_stC))

#define SENSOR_PREFIX1	SENSOR_A/(2*SENSOR_B)
#define SENSOR_PREFIX2	(SENSOR_A*SENSOR_A) / ( 4 * SENSOR_B*SENSOR_B)
#define SENSOR_DEN		( SENSOR_RH0 * SENSOR_B)


void OSC_init(void);
void TC0_init(void);

inline uint16_t meas_voltage_mv(void)
{
	return ADCA_result_mV(ADC_V_CH);
};
inline uint16_t meas_current_ma(void)
{
	return (ADCA_result_mV(ADC_I_CH) * 1000) / CURRENT_R_mOHM;
};

inline uint16_t to_temperature(uint16_t U_mV, uint16_t I_mA)
{
	float R = U_mV/I_mA;
	
	return -(SENSOR_PREFIX1 + sqrt( SENSOR_PREFIX2 - (SENSOR_RH0 - R) / SENSOR_DEN) );
};

volatile uint8_t flag=0;
uint32_t i = 0;
uint32_t N = 0;

uint16_t U = 0;
uint16_t I = 0;

uint32_t x = 0;
uint32_t a = 0;
int main(void)
{
	
	OSC_init();
	TC0_init();

	ADC_init(ADC_V_I_NAME,ADC_DIV512,ADC_REF_INT1V,ADC_MODE_SIGNED,ADC_FREERUN_ENABLE,ADC_SWEEP_CH01);
	ADC_CH_init(ADC_V_I_NAME,ADC_V_CH, ADC_INPUTMODE_DIFF,ADC_POSPIN5,ADC_NEGPIN1,0);
	ADC_CH_init(ADC_V_I_NAME,ADC_I_CH, ADC_INPUTMODE_DIFF,ADC_POSPIN6,ADC_NEGPIN2,0);	
	ADC_enable(ADC_A);
	
	lcd_init();
	DAC_init(DAC_CHDUAL,DAC_REF_INT1V);
	
	
lcd_int(x);

	

	DIROUT(led);
	DIRIN(sw0);
	PULLUP(sw0);


	
	N = signal_param(100,900,100,F_SAMPLE);
	//x=0;
	a = to_temperature(10000,50);
				
		
    while (1) 
    {

		if(flag)
		{		
			U = meas_voltage_mv();
			I = meas_current_ma();

			
			DAC_CH0_mV(sinus_sample(i));
			
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
	++x;
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



