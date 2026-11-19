/*
  MIC2 Week 5. Analog

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
#include "avr/io.h"

#include "adc.h"

void adc_init(void) {
  // Initialize the ADC peripheral
  // ADC Multiplexer Selection Register:
  //   REFS[1:0] = 01   : AV_CC with external capacitor at AREF pin
  //   ADLAR     = 0    : Right adjust result
  //   MUX[3:0]  = 0000 : Select channel ADC0
  ADMUX = (1 << REFS0);

  // ADC Control and Status Register A:
  //   ADEN      = 1   : ADC enable
  //   ADSC      = 0   : Do not start a conversion
  //   ADATE     = 0   : Auto triggering disabled
  //   ADIF      = 0   : Do not clear the interrupt flag
  //   ADIE      = 0   : Do not enable interrupts
  //   ADPS[2:0] = 111 : Prescaler division factor 128
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
  // Check if channel supported
  // Note. Cannot support the temperature sensor (ADC8), becasue it
  // requires a different reference voltage. Measurements point out
  // that switching between reference voltages does not work.
  if (channel > 7) {
    return 0;
  }

  // Select channel
  ADMUX = (1 << REFS0) | (channel);

  // Start a conversion (making sure all other bits do not change)
  ADCSRA |= (1 << ADSC);

  // Wait for the conversion to complete by polling the ADSC bit
  while ((ADCSRA & (1 << ADSC)) != 0) { ; }

  // Read the result
  uint16_t val = ADC;

  return val;
}
