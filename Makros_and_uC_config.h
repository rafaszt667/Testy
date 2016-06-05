/*
 * Makros_and_uCinit.h
 *
 * Created: 2016-03-23 18:13:01
 *  Author: Rafał
 */ 
//Makra ulatwiajace dostep do portow i funkcje konfiguracji procesora

/*
	Przykład: 
	#define led led			//definiujemy nazwe pinu, zapobiega powtorzeniiom nazw, a kompilator proponuje podczas wpisywania
	#define ledpin 0		//numer pinu
	#define ledport D		//Litera portu
	
	Użycie:
	
	DIROUT(led);			//pin led jako wyjście
	SET(led);				//ustaw 1 na pinie led
	CLR(led);				//ustaw 0 na pinie led
	...
*/

#ifndef MAKROS_AND_UC_CONFIG_H_
#define MAKROS_AND_UC_CONFIG_H_

//Wybór kierunku pinu:
#define DIROUT(x)		 SDIROUT(PORT(x), PIN_bm(x))
#define SDIROUT(port,bm) KDIROUT(port,bm)
#define KDIROUT(port,bm) port.DIRSET = bm

#define DIRIN(x)		SDIRIN(PORT(x), PIN_bm(x))
#define SDIRIN(port,bm)	KDIRIN(port,bm)
#define KDIRIN(port,bm)	port.DIRCLR = bm
/*****************************************************/


//Ustawianie stanu logicznego na pinie:
#define SET(x)			SSET(PORT(x),PIN_bm(x))
#define SSET(port,bm)	KSET(port,bm)
#define KSET(port,bm)	port.OUTSET = bm

#define CLR(x)			SCLR(PORT(x),PIN_bm(x))
#define SCLR(port,bm)	KCLR(port,bm)
#define KCLR(port,bm)	port.OUTCLR = bm

#define TGL(x)			STGL(PORT(x),PIN_bm(x))
#define STGL(port,bm)	KTGL(port, bm)
#define KTGL(port, bm)	port.OUTTGL = bm
/****************************************************/


//Odczyt stanu logicznego na pinie wejsciowym
#define INPUT(x)		 SINPUT(PORT(x), PIN_bm(x))
#define SINPUT(port,bm)  KINPUT(port, bm)
#define KINPUT(port, bm) (port.IN & bm)
/****************************************************/


//Pullup na pin
#define PULLUP(x)				SPULLUP(PORT(x), PINCTRL(x))
#define SPULLUP(port,pinctrl)	KPULLUP(port, pinctrl)
#define KPULLUP(port, pinctrl)	port.pinctrl  = PORT_OPC_PULLUP_gc

//Pulldown na pin
#define PULLDOWN(x)					SPULLDOWN(PORT(x), PINCTRL(x))
#define SPULLDOWN(port,pinctrl)		KPULLDOWN(port, pinctrl)
#define KPULLDOWN(port, pinctrl)	port.pinctrl  = PORT_OPC_PULLUP_gc
/****************************************************/


//Zwraca nazwe rejestru konfiguracyjnego pinu
#define PINCTRL(x)	SPINCTRL(x##pin)
#define SPINCTRL(x) KPINCTRL(x)
#define KPINCTRL(x) PIN##x##CTRL
/****************************************************/


//Zwraca nazwe portu pinu
#define PORT(x)		SPORT(x##port)
#define SPORT(x)	KPORT(x)
#define KPORT(x)	PORT##x

//Zwraca beat map pinu (PINx_bm
#define PIN_bm(x)	SPIN_bm(x##pin)
#define SPIN_bm(x)	KPIN_bm(x)
#define KPIN_bm(x)	PIN##x##_bm
/****************************************************/

typedef enum bool
{
	FALSE = 0,
	TRUE = 1
} bool_t;
#endif /* MAKROS_AND_UC_CONFIG_H_ */