/*
  MIC2 Week 3. Timers/Counters

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer0.h"

static volatile uint32_t ms = 0;

void timer0_init(void) {
  ms = 0;

  // Configure Timer/counter 0 to generate an interrupt every millisecond
  //
  // TODO: Explain why the following settings configure
  //       Timer/counter 0 to generate an interrupt every millisecond.
  //
  OCR0A = 249;
  TCCR0A = (1 << WGM01);
  TCCR0B = (1 << CS01) | (1 << CS00);
  TIMSK0 = (1 << OCIE0A);
}

// Interrupt Service Routine that is automatically executed as soon as
// timer/counter 0 has reached its compare value
ISR(TIMER0_COMPA_vect) {
  ms++;
}

uint32_t timer0_millis(void) {
  // ms is a 32-bit variable (uint32_t). This means that multiple accesses
  // are needed to read or write the value of ms. There is a chance that
  // in the middle of these multiple accesses, the ms value is written due
  // to the ISR being triggered. In order to make sure the value of ms is
  // not updated when reading it, disable interrupts while reading the
  // value.
  cli();
  uint32_t ret = ms;
  sei();

  return ret;
}
