/*
  MIC2 Week 1. Basics

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Configure PB5 so it will be an output pin
  DDRB |= (1 << DDB5);

  while (1) {
    // LED connected to PB5 on and wait
    PORTB |= (1 << PORTB5);
    _delay_ms(100);

    // LED connected to PB5 off and wait
    PORTB &= ~(1 << PORTB5);
    _delay_ms(100);
  }
}