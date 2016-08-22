/** @file ADC.h
	@brief Obsługa ADC.
		Zawira deklaracje funkcji do konfiguracji i obsługi przetwornika ADC. 
		Inicjalizacja wybranego przetwornika, odczyt napięcia, ustawienie offsetu, włączenie wybranego przetwornika.
	@author Rafał Sztwiertnia
	*/

#ifndef ADC_H_
#define ADC_H_

/// Wartość napięcia odniesienia dla przetwornika A w mV.
#define ADCA_ref_source_val_mV 1000

/// Wartość napięcia odniesienia dla przetwornika B w mV.
#define ADCB_ref_source_val_mV 1000

/// Wybór ADC (A lub B)
typedef enum ADC_name
{
	ADC_A = 0,
	ADC_B = 1
}ADC_name_t;
	
/// Wybór dzielnika preskalera dla częstotliwości zegara ADC
typedef enum ADC_prescaler
{
	ADC_DIV4	= ADC_PRESCALER_DIV4_gc,	///< Dzielenie przez 4
	ADC_DIV8	= ADC_PRESCALER_DIV8_gc,	///< Dzielenie przez 8
	ADC_DIV16	= ADC_PRESCALER_DIV16_gc,	///< Dzielenie przez 16
	ADC_DIV32	= ADC_PRESCALER_DIV256_gc,	///< Dzielenie przez 32
	ADC_DIV64	= ADC_PRESCALER_DIV64_gc,	///< Dzielenie przez 64
	ADC_DIV128	= ADC_PRESCALER_DIV128_gc,	///< Dzielenie przez 128
	ADC_DIV256	= ADC_PRESCALER_DIV256_gc,	///< Dzielenie przez 256
	ADC_DIV512	= ADC_PRESCALER_DIV512_gc	///< Dzielenie przez 512
}ADC_prescaler_t;

/// Wybór napięcia referencyjnego ADC
typedef enum ADC_refference
{
	ADC_REF_INT1V		= ADC_REFSEL_INT1V_gc,		///< Wewnętrzne źródło 1V
	ADC_REF_VCC_DIV1_6	= ADC_REFSEL_INTVCC_gc,		///< Napięcie zasilania podzielone przez 1,6 (VCC/1,6)
	ADC_REF_VCC_DIV2	= ADC_REFSEL_INTVCC2_gc,	///< Napięcie zasilania podzielone przez 2 (VCC/2)
	ADC_REF_AREFA		= ADC_REFSEL_AREFA_gc,		///< Źródło podłączone do pinu AREFA
	ADC_REF_AREFB		= ADC_REFSEL_AREFB_gc		///< Źródło podłączone do pinu AREFB
} ADC_refference_t;

/// Wybór metody konwersji (ze znakiem/ bez znaku)
typedef enum ADC_conv_mode
{
	ADC_MODE_UNSIGNED	= 0,	///< Konwersja bez znaku 
	ADC_MODE_SIGNED		= 1		///< Konwersja ze znakiem
} ADC_conv_mode_t;

/// Włączenie/Wyłączenie trybu freerun ADC
typedef enum ADC_freerun
{
	ADC_FREERUN_DISABLE = 0,	///< Tryb freerun wyłączony
	ADC_FREERUN_ENABLE	= 1		///< Tryb freerun włączony
} ADC_freerun_t;

/// Wybór kanałów do których ma być podłaczony trigger dla trybu freerun
typedef enum ADC_sweep
{
	ADC_SWEEP_CH0	= ADC_SWEEP_0_gc,		///< Trigger do kanału 0
	ADC_SWEEP_CH01	= ADC_SWEEP_01_gc,		///< Trigger do kanału 0 i 1
	ADC_SWEEP_CH012	= ADC_SWEEP_012_gc,		///< Trigger do kanału 0, 1 i 2
	ADC_SWEEP_CH0123 = ADC_SWEEP_0123_gc	///< Trigger do kanału 0, 1, 2 i 3
} ADC_sweep_t;

/// Wybór kanału przetwornika (0, 1, 2, 3)
typedef enum ADC_channel
{
	ADC_CH0,
	ADC_CH1,
	ADC_CH2,
	ADC_CH3
} ADC_channel_t;

/// Wybór rodzaju wejścia przetwornika.
typedef enum ADC_inputmode
{
	ADC_INPUTMODE_INTERNAL		= ADC_CH_INPUTMODE_INTERNAL_gc,		///< Wejście wewnętrzne - wewnętrzny termometr, wyjście przetwornika...
	ADC_INPUTMODE_SINGLEENDED	= ADC_CH_INPUTMODE_SINGLEENDED_gc,	///< Wejście pojedyńcze
	ADC_INPUTMODE_DIFF			= ADC_CH_INPUTMODE_DIFF_gc,			///< Wejście różnicowe
	ADC_INPUTMODE_DIFFWGAIN		= ADC_CH_INPUTMODE_DIFFWGAIN_gc		///< Wejście różnicowe z wzmocnieniem 
} ADC_inputmode_t;

/// Podłączenie wejścia nieodwracającego (do jakiego pinu)
typedef enum ADC_possitive_pin
{
	ADC_POSPIN0	= ADC_CH_MUXPOS_PIN0_gc,	///< pin 0 
	ADC_POSPIN1	= ADC_CH_MUXPOS_PIN1_gc,	///< pin 1 
	ADC_POSPIN2	= ADC_CH_MUXPOS_PIN2_gc,	///< pin 2 	
	ADC_POSPIN3	= ADC_CH_MUXPOS_PIN3_gc,	///< pin 3 
	ADC_POSPIN4	= ADC_CH_MUXPOS_PIN4_gc,	///< pin 4
	ADC_POSPIN5	= ADC_CH_MUXPOS_PIN5_gc,	///< pin 5
	ADC_POSPIN6	= ADC_CH_MUXPOS_PIN6_gc,	///< pin 6
	ADC_POSPIN7	= ADC_CH_MUXPOS_PIN7_gc,	///< pin 7
	ADC_POSPIN8	= ADC_CH_MUXPOS_PIN8_gc,	///< pin 8
	ADC_POSPIN9	= ADC_CH_MUXPOS_PIN9_gc,	///< pin 9
	ADC_POSPIN10	= ADC_CH_MUXPOS_PIN10_gc,	///< pin 10
	ADC_POSPIN11	= ADC_CH_MUXPOS_PIN11_gc,	///< pin 11
	ADC_POSPIN12	= ADC_CH_MUXPOS_PIN12_gc,	///< pin 12
	ADC_POSPIN13	= ADC_CH_MUXPOS_PIN13_gc,	///< pin 13
	ADC_POSPIN14	= ADC_CH_MUXPOS_PIN14_gc,	///< pin 14
	ADC_POSPIN15	= ADC_CH_MUXPOS_PIN15_gc	///< pin 15
} ADC_possitive_pin_t;

/// Podłączenie wejścia odwracającego (do pinu lub do GND)
/// Wybór pinu jest zależny od trybu (różnicowy ze wzmocnieniem lub bez)
/// @see ADC_inputmode_t
typedef enum ADC_negative_pin
{
	ADC_NEGPIN0	= ADC_CH_MUXNEG_PIN0_gc,	///< pin 0 (Tylko dla trybu różnicowego bez wzmocnienia)
	ADC_NEGPIN1	= ADC_CH_MUXNEG_PIN1_gc,	///< pin 1 (Tylko dla trybu różnicowego bez wzmocnienia)
	ADC_NEGPIN2	= ADC_CH_MUXNEG_PIN2_gc,	///< pin 2 (Tylko dla trybu różnicowego bez wzmocnienia)
	ADC_NEGPIN3	= ADC_CH_MUXNEG_PIN3_gc,	///< pin 3 (Tylko dla trybu różnicowego bez wzmocnienia)
	ADC_NEGPIN4	= ADC_CH_MUXNEG_PIN4_gc,	///< pin 4 (Tylko dla trybu różnicowego ze wzmocnieniem)
	ADC_NEGPIN5	= ADC_CH_MUXNEG_PIN5_gc,	///< pin 5 (Tylko dla trybu różnicowego ze wzmocnieniem)
	ADC_NEGPIN6	= ADC_CH_MUXNEG_PIN6_gc,	///< pin 6 (Tylko dla trybu różnicowego ze wzmocnieniem)
	ADC_NEGPIN7	= ADC_CH_MUXNEG_PIN7_gc,	///< pin 7 (Tylko dla trybu różnicowego ze wzmocnieniem)
	
	ADC_GND_DIFF		= ADC_CH_MUXNEG_GND_MODE3_gc,		///< Podłączenie do padu GND dla trybu różnicowego bez wzmocnienia
	ADC_GND_DIFFWGAIN	= ADC_CH_MUXNEG_GND_MODE4_gc,		///< Podłączenie do padu GND dla trybu różnicowego z wzmocnieniem
	ADC_INTGND_DIFF		= ADC_CH_MUXNEG_INTGND_MODE3_gc,	///< Podłączenie do wewnętrznego GND dla trybu różnicowego bez wzmocnienia
	ADC_INTGND_DIFFWGAIN = ADC_CH_MUXNEG_INTGND_MODE4_gc	///< Podłaczenie do wewnętrzengo GND dla trybu różnicowego z wzmocnieniem
} ADC_negative_pin_t;

///Ustawienie wzmocnienia kanału. Możliwe tylko dla trybu różnicowego ze wzmocnieniem.
///@see ADC_inputmode_t
typedef enum ADC_gain
{
	ADC_GAIN_1X		= ADC_CH_GAIN_1X_gc,	///< 1x gain
	ADC_GAIN_2X		= ADC_CH_GAIN_2X_gc,	///< 2x gain
	ADC_GAIN_3X		= ADC_CH_GAIN_4X_gc,	///< 4x gain
	ADC_GAIN_8X		= ADC_CH_GAIN_8X_gc,	///< 8x gain
	ADC_GAIN_16X	= ADC_CH_GAIN_16X_gc,	///< 16x gain
	ADC_GAIN_32X	= ADC_CH_GAIN_32X_gc,	///< 32x gain
	ADC_GAIN_64X	= ADC_CH_GAIN_64X_gc,	///< 64x gain
	ADC_GAIN_DIV2	= ADC_CH_GAIN_DIV2_gc,	///< x/2 gain
} ADC_gain_t;



/** @brief Funkcja inicjalizacji przetwornika ADC.
	Umożliwia konfigurację przetwornika, ustawienia preskalera, napięcia odniesienia, metody konwersji, 
	włączenie/wyłączenie trybu freerun i kanałów do których ma być podłączony trigger dla trybu freerun.
	
	@param ADCx			Wybór przetwornika (A lub B).
	@param prescaler	Wybór dzielnika częstotliwości zegara danego przetwornika.
	@param refference	Wybór napięcia odniesienia dla danego przetwornika.
	@param mode			Wybór metody konwersji (ze znakiem lub bez znaku)
	@param freerun		Umożliwia włączenie trybu freerun przetwornika
	@param sweep		Wybór kanałów do których ma być podłaczony trigger dla trybu freerun 
*/
void ADC_init(ADC_name_t  adc_name, ADC_prescaler_t prescaler, ADC_refference_t refference, ADC_conv_mode_t mode, ADC_freerun_t freerun, ADC_sweep_t sweep);

/**	@Brief Funkcja inicjalizacji kanału przetwornika ADC
	Umozliwia konfiguracje danego kanału przetwornika ADC -  wybranie metody pomiaru,  
	podłączenie pinów czy ustawienie wzmocnienia
	
	@param ADCx				Wybór przetwornika (A lub B)
	@param CHx				Wybór kanału przetwornika
	@param inputmode		Wybór metody pomiaru (pojedyńczy lub różnicowy, róznicowy ze wzmocnieniem)
	@param possitive_pin	Podłaczenie pinu dodatniego kanału
	@param negative_pin		Podłaczenie pinu negatywnego kanału
	@param gain				Wzmocnienie dla metody róznicowe ze wzmocnieniem
	*/
void ADC_CH_init(ADC_name_t adc_name, ADC_channel_t ch_name,  ADC_inputmode_t inputmode, ADC_possitive_pin_t possitive_pin, ADC_negative_pin_t negative_pin, ADC_gain_t gain);

/** @brief Funkcja zwraca wynik pomiaru z przetwornika A.

	@param ch_number		Wybór kanału przetwornika A
	
	@return Wynik pomiaru z danego kanału
*/
int16_t ADCA_result_mV(ADC_channel_t ch_number);

/**	@brief Funkcja zwara wynik pomiaru z przetwornika B.

	@param ch_number		Wybór kanału przetwornika B
	
	@return Wynik pomiaru z danego kanału 
*/
int16_t ADCB_result_mV(ADC_channel_t ch_number);

/**	@brief Włączenie ADC
	Nalezy użyć po ADC_init(...);
	
	@param adc_name			Wybór przetwornika (A lub B)
*/
 void ADC_enable(ADC_name_t adc_name);
 
/**	@brief Wyłączenie ADC
	Pozwala wyłaczyć ADC.
	
	@param adc_name			Wybór przetwornika (A lub B)
*/
 void ADC_disable(ADC_name_t adc_name);
#endif /* ADC_H_ */