/*
  MIC2 SoftSerial example

  Author Hugo Arends

  Date   02/12/2025
*/
#include <avr/io.h>
#include "SoftSerial.h"

int main(void) {
  DDRB = (1 << DDB5);

  SoftSerialInit();
  sei();

  SoftSerialTransmitString("\r\nSoftSerial demo\r\n");

  while (1) {
    if (SoftSerialUnread() > 0) {
      char data = SoftSerialReceiveByte();

      switch (data) {
        case '1':
          PINB = (1 << PINB5);
          break;

        case 'h':
          SoftSerialTransmitString("hello world!\r\n");
          break;

        case 'H':
          SoftSerialTransmitString("HELLO WORLD!\r\n");
          break;

        default:
          SoftSerialTransmitString("Unknown command\r\n");
          break;
      }
    }
  }
}
