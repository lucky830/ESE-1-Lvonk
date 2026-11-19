/**
* \file
*
* \brief USART Interrupt example
*
* Copyright (C) 2016 Atmel Corporation. All rights reserved.
* Modified by H. Arends for HAN University of Applied Sciences to support
* the ATmega328P microcontroller.
*
* \page License
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Atmel may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an
* Atmel microcontroller product.
*
* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/
/*
* Support and FAQ: visit
* <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart0.h"

// UART Buffer Defines
// Supported sizes: 2,4,8,16,32,64,128 or 256 bytes
#define USART_RX_BUFFER_SIZE (64)
#define USART_TX_BUFFER_SIZE (64)

// Buffer size checks
#define USART_RX_BUFFER_MASK (USART_RX_BUFFER_SIZE - 1)
#if (USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK)
#error RX buffer size is not a power of 2
#endif

#define USART_TX_BUFFER_MASK (USART_TX_BUFFER_SIZE - 1)
#if (USART_TX_BUFFER_SIZE & USART_TX_BUFFER_MASK)
#error TX buffer size is not a power of 2
#endif

// Static variables
static char USART_RxBuf[USART_RX_BUFFER_SIZE];
static volatile unsigned char USART_RxHead;
static volatile unsigned char USART_RxTail;

static char USART_TxBuf[USART_TX_BUFFER_SIZE];
static volatile unsigned char USART_TxHead;
static volatile unsigned char USART_TxTail;

/*
 * Initialize the USART.
 */
void usart0_init(void) {
  // Configure the baud rate
  UBRR0 = (uint16_t)((F_CPU / 16UL / 9600) - 1);

  // Enable USART receiver and transmitter
  UCSR0B = ((1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0));

  // Flush buffers
  unsigned char x = 0;
  USART_RxTail = x;
  USART_RxHead = x;
  USART_TxTail = x;
  USART_TxHead = x;

  sei();
}

/*
 * Interrupt handler for received data.
 * Data is placed in the receive buffer.
 */
ISR(USART_RX_vect) {
  char data;
  unsigned char tmphead;

  // Read the received data
  data = UDR0;

  // Calculate buffer index
  tmphead = (USART_RxHead + 1) & USART_RX_BUFFER_MASK;

  // Store new index
  USART_RxHead = tmphead;

  if (tmphead == USART_RxTail) {
    // ERROR! Receive buffer overflow
  }

  // Store received data in buffer
  USART_RxBuf[tmphead] = data;
}

/*
 * Interrupt handler for transmit data.
 * Data is read from the transmit buffer. If all data was transmitted,
 * transmit interrupts are disabled.
 */
ISR(USART_UDRE_vect) {
  unsigned char tmptail;

  // Check if all data is transmitted
  if (USART_TxHead != USART_TxTail) {
    // Calculate buffer index
    tmptail = (USART_TxTail + 1) & USART_TX_BUFFER_MASK;

    // Store new index
    USART_TxTail = tmptail;

    // Start transmission
    UDR0 = USART_TxBuf[tmptail];
  } else {
    // Disable UDRE interrupt
    UCSR0B &= ~(1 << UDRIE0);
  }
}

/*
 * This function returns a new byte from the receive buffer.
 * It will wait for data to be available! Use the function
 * usart0_nUnreadBytes() to make sure data is available.
 */
char usart0_receive(void) {
  unsigned char tmptail;

  // Wait for incoming data
  while (USART_RxHead == USART_RxTail) { ; }

  // Calculate buffer index
  tmptail = (USART_RxTail + 1) & USART_RX_BUFFER_MASK;

  // Store new index
  USART_RxTail = tmptail;

  // Return data
  return USART_RxBuf[tmptail];
}

/*
 * This function places a new byte in the transmit buffer
 * and starts a new transmission by enabling transmit interrupt.
 */
void usart0_transmit(char data) {
  unsigned char tmphead;

  // Calculate buffer index
  tmphead = (USART_TxHead + 1) & USART_TX_BUFFER_MASK;

  // Wait for free space in buffer
  while (tmphead == USART_TxTail) { ; }

  // Store data in buffer
  USART_TxBuf[tmphead] = data;

  // Store new index
  USART_TxHead = tmphead;

  // Enable UDRE interrupt
  UCSR0B |= (1 << UDRIE0);
}

/*
 * This function returns the number of unread bytes in the receive buffer.
 */
unsigned char usart0_nUnread(void) {
  if (USART_RxHead == USART_RxTail)
    return 0;
  else if (USART_RxHead > USART_RxTail)
    return USART_RxHead - USART_RxTail;
  else
    return USART_RX_BUFFER_SIZE - USART_RxTail + USART_RxHead;
}

/*
 * Transmits a string of characters to the USART.
 * The string must be terminated with '\0'.
 *
 * - This function uses the function usart0_transmit() to
 *   transmit a byte via the USART
 * - Bytes are transmitted until the terminator
 *   character '\0' is detected. Then the function returns.
 */
void usart0_transmitStr(const char *str) {
  while (*str) {
    usart0_transmit(*str++);
  }
}

/*
 * Transmits an integer by converting it to an ASCII string.
 *
 * - This function uses the function usart0_transmit() to
 *   transmit a byte via the USART
 */
void usart0_transmitInt(int i) {
  signed char t = 0;
  char h[16];  // Max 16 digit numbers supported

  // Is i negative?
  if (i < 0) {
    // Print minus sign
    usart0_transmit('-');
    i = -i;
  }

  // Populate buffer
  do {
    h[t++] = (i % 10) + '0';
    i = i / 10;
  } while ((i > 0) && (t < 16));

  // Check for overflow
  if (t >= 16) {
    return;
  }

  // Print buffer in reverse order
  while (t > 0) {
    usart0_transmit(h[--t]);
  }
}
