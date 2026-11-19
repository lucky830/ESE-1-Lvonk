/*
  MIC2 Week 4. Communication

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
#include "util/delay.h"
#include "timer0.h"
#include "usart0.h"

int cnt = -10;

int main(void) {
  // PB0 - PB5 output, rest input
  DDRB = 0b00111111;

  // Initialize the millisecond counter
  timer0_init();
  uint32_t previousmillis = 0;
  uint32_t currentmillis = 0;
  uint32_t interval = 1000;

  // Initialize the USART
  usart0_init();

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

      // Print counter
      usart0_transmitStr("cnt = ");
      usart0_transmitInt(cnt);
      usart0_transmitStr("\n");

      cnt++;
    }

    // ------------------------------------------------------------------------

    // Check for unread bytes in the receive buffer
    unsigned char nBytes = usart0_nUnread();

    if (nBytes > 0) {
      // char data = usart0_receive();

      // TODO: Handle the data
    }
  }

  return 0;
}
