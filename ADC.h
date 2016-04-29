/** @file ADC.h
	@brief Obsługa ADC.
		Zawira deklaracje funkcji do konfiguracji i obsługi przetwornika ADC. 
		Inicjalizacja wybranego przetwornika, odczyt napięcia, ustawienie offsetu, włączenie wybranego przetwornika.
	@author Rafał Sztwiertnia
	*/

#ifndef ADC_H_
#define ADC_H_


/// Wybór ADC (A lub B)
typedef enum ADC_name
{
	A,
	B
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

/// Wybór napięcie referencyjnego ADC
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
void ADC_init(ADC_name_t ADCx, ADC_prescaler_t prescaler, ADC_refference_t refference, ADC_conv_mode_t mode, ADC_freerun_t freerun, ADC_sweep_t sweep);
#endif /* ADC_H_ */