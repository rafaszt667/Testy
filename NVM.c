/*
 * NVM.c
 *
 * Created: 2016-03-28 16:06:39
 *  Author: Rafał
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "NVM.h"

uint8_t ReadCalibrationByte(uint16_t addr)
{
	uint8_t result;
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	
	result = pgm_read_byte(addr);
	
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;
	return result;
}