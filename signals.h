/*
 * signals.h
 *
 * Created: 2016-04-07 22:31:44
 *  Author: Rafał
 */ 


#ifndef SIGNALS_H_
#define SIGNALS_H_

/*	@brief Funkcja ustawia parametry generowanego przebiegu.
	Na podstawie podanych parametrow zostaje obliczony przebieg. Funkcję należy wykonać przed generowaniem jakiego kolwiek przebiegu.
	Możliwa jest zmiana parametrów w dowolnej chwili. Zwaraca liczbę próbek w generowanym okresie.
	
	@param f_signal_mHz	Częstotliwość generowanego przebiegu.
	@param max_mV		Wartość maksymalna generowanego przebiegu w mV
	@param min_mV		Wartosć minimalna generowanego przebiegu w mV
	@param f_sample_Hz	Częstotliwość próbkowania w Hz.

	@return Liczba próbek w okresie generowanego przebiegu.
*/
uint32_t signal_param(uint32_t f_signal_mHz, uint16_t max_mV, uint16_t min_mV, uint16_t f_sample_Hz);

/*	@brief Pobranie wskazanej próbki przebiegu sinusoidalnego.
	Pozwala generować przebieg sinusoidalny, zwraca wartość przebiegu dla wskazanego numeru próbki.
	
	@param sample Numer próbki dla której ma być zwrócona wartość przebiegu.
	@return Wartość dla podanego numeru próbki przebiegu sinusoidalnego.
*/
uint16_t sinus_sample(uint32_t sample);

/*	@brief Pobranie wskazanej próbki przebiegu trójkątnego.
	Pozwala generować przebieg trójkątny, zwraca wartość przebiegu dla wskazanego numeru próbki.
	
	@param sample Numer próbki dla której ma być zwrócona wartość przebiegu.
	@return Wartość dla podanego numeru próbki przebiegu trójkątnego.
*/
uint16_t triangle_sample(uint16_t sample);

/*	@brief Pobranie wskazanej próbki przebiegu piłokształnego.
	Pozwala generować przebieg piłokształtny, zwraca wartość przebiegu dla wskazanego numeru próbki.
	
	@param sample Numer próbki dla której ma być zwrócona wartość przebiegu.
	@return Wartość dla podanego numeru próbki przebiegu piłokształtnego.
*/
uint16_t sowtooth_sample(uint16_t sample);

#endif /* SIGNALS_H_ */