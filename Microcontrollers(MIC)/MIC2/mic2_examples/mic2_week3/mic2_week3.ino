/*
  MIC2 Week 3. Timers/Counters

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/

#include <avr/io.h>

#include "timer0.h"

int main(void) {
  // PB5 output
  DDRB |= (1 << PORTB5);

  // PB7 input
  DDRB &= ~(1 << DDB7);

  // Initialize the millisecond counter
  timer0_init();

  // Global interrupt enable
  sei();

  uint32_t previousmillis = 0;
  uint32_t currentmillis = 0;
  uint32_t interval = 100;

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
    }
  }
}
