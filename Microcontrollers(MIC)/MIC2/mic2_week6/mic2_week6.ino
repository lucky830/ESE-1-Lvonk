/*
  MIC2 Week 6. Analog

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
#include "adc.h"
#include "timer0.h"
#include "usart0.h"

int cnt = 0;

int main(void) {
  // PB0 - PB5 output, rest input
  DDRB = 0b00111111;

  // Initialize the millisecond counter
  timer0_init();
  uint32_t previousmillis = 0;
  uint32_t currentmillis = 0;
  uint32_t interval = 200;

  // Initialize the USART
  usart0_init();

  // Initialize ADC
  adc_init();

  sei();

  while (1) {

    // Get the current millis
    currentmillis = timer0_millis();

    // Has 'interval' time passed since the last time we checked?
    if ((currentmillis - previousmillis) >= interval) {
      // Yes, 'interval' time has passed
      // Save the current millis
      previousmillis = currentmillis;

      // Toggle the LED
      PORTB ^= (1 << PORTB5);

      // Get ADC reading
      uint16_t adc0_val = adc_read(0);

      // Print counter
      usart0_transmitStr("adc0 = ");
      usart0_transmitInt(adc0_val);
      usart0_transmitStr("\n");
    }
  }
}
