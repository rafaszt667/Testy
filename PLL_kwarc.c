/*
 * Testy.c
 *
 * Created: 2016-03-22 19:23:38
 * Author : Rafał
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define LedPort PORTD
#define LedPin PIN0_bm

int main(void)
{
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc|			//konfiguracja kwarcu
					OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL = OSC_XOSCEN_bm;						//wlaczenie kwarcu
	

	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc|				//zrodlo pll i mnożnik
	2;
			
	OSC.CTRL |= OSC_PLLEN_bm;						//wlaczenie pll NIE SKASOWAC WLACZONEGO KWACU!!!
	while(!(OSC.STATUS & OSC_PLLRDY_bm));			//czekaj az bedzie stabilna pll (dobrze dac do petli bo zewnetrzny kwarc moze nie ruszyc)
			
	CPU_CCP = CCP_IOREG_gc;							//wlaczenie edycji waznych rejestrow
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;					//pll jako zrodko sygnalu zegarowego

	
	
	
	
	
	LedPort.DIRSET = LedPin;
	LedPort.OUTSET = LedPin;
	   
    while (1) 
    {
		LedPort.OUTTGL = LedPin;
		_delay_ms(1000);
    }
}

