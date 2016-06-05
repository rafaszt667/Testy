/*
 * DAC.h
 *
 * Created: 2016-03-28 13:14:02
 *  Author: Rafał
 */ 


#ifndef DAC_H_
#define DAC_H_


#define DAC_CH0_offset_cal_mV 11	///<Offset dla kanału 0
#define DAC_CH1_offset_cal_mV 0		///<Offset dla kanału 1

#define DAC_CH0_gainerr_cal 0.969	///<Błąd wzmocnienia kanału 0
#define DAC_CH1_gainerr_cal 1		///<Błąd wzmocnienia kanału 1

#define DAC_ref_source_val_mV 1000	///<Wartość napięcia referencyjnego dla przetwornika

///Wybór kanału przetwornika
typedef enum DAC_channel
{
	DAC_CH0		= DAC_CHSEL_SINGLE_gc,	///<Tylko kanał 0
	DAC_CH1		= DAC_CHSEL_SINGLE1_gc,	///<Tylko kanał 1
	DAC_CHDUAL	= DAC_CHSEL_DUAL_gc		///<Dwa kanały 0 i 1	
}DAC_channel_t;

///Wybór napięcia referencyjnego przetwornika
typedef enum DAC_refference
{
	DAC_REF_INT1V	= DAC_REFSEL_INT1V_gc,	///<Wewnętrzne źródło 1V
	DAC_REF_AVCC	= DAC_REFSEL_AVCC_gc,	///<Napięcie zaislania AVCC
	DAC_REF_AREFA	= DAC_REFSEL_AREFA_gc,	///<Napięcie z pinu AREFA
	DAC_REF_AREFB	= DAC_REFSEL_AREFB_gc	///<Napięcie z pinu AREFB
}DAC_refference_t;

/** @brief Funkcja inicjalizacji przetwornika DAC.
	Umożliwia konfigurację przetwornika, Wybranie kanału lub kanałów do konwersji, wybranie źródła napięcia referencyjnego.
	
	@param ch_select	Wybór kanału lub kanałów do konwersji.
	@param refference	Wybór źródła napięcia referencyjnego. 
*/
void DAC_init(DAC_channel_t ch_select, DAC_refference_t refference );

/** @brief Ustawienie wartości napięcia na wyjściu kanału 0.
	
	@param	mV	Napięcie jakie ma być ustawione na wyjściu przetwornika kanalu 0
	*/
void DAC_CH0_mV(uint16_t mV);
/** @brief Ustawienie wartości napięcia na wyjściu kanału 1.
	
	@param	mV	Napięcie jakie ma być ustawione na wyjściu przetwornika kanalu 1
	*/
void DAC_CH1_mV(uint16_t mV);

#endif /* DAC_H_ */