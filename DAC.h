/*
 * DAC.h
 *
 * Created: 2016-03-28 13:14:02
 *  Author: Rafał
 */ 


#ifndef DAC_H_
#define DAC_H_
#define DAC_CH0_offset_cal_mV 11
#define DAC_CH1_offset_cal_mV 
#define DAC_CH0_gainerr_cal 0.969
#define DAC_CH1_gainerr_cal


#define DAC_ref_source_val_mV 1000

void DAC_init(DAC_CHSEL_t CH_SEL, DAC_REFSEL_t refference );
void DAC_CH0_mV(uint16_t mV);
void DAC_CH1_mV(uint16_t mV);

#endif /* DAC_H_ */