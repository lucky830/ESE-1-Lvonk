/*
  MIC2 Week 5. Analog

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adc_init(void);
uint16_t adc_read(uint8_t channel);

#endif  // ADC_H
