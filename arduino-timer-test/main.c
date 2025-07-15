/*
 * Copyright 2025 jsk AGPL3
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#define UART_BAUD 115200
#define SOFT_BAUD 300
#define SUBSAMPLE_LN2   2
#define SUBSAMPLE   (1 << SUBSAMPLE_LN2)

uint8_t recv_ccount = 0;
uint8_t recv_buf = 0;
uint8_t last_recv = 0;
uint8_t new_last_recv = 0;
uint8_t last_recv_packet_start = 0;

uint8_t tx_ccount = 0;
uint8_t tx_buf = 0;
uint8_t new_tx = 0;
// Double buffer the tx buffer
uint8_t next_to_tx = 0;
uint8_t new_next_to_tx = 0;

void _putc(uint8_t c) {
  while ((UCSR0A & (1 << UDRE0)) == 0) { }
  UDR0 = c;
}


void listen() {
  uint8_t pb4 = (PINB & (1 << PB4)) ? 1 : 0;

  if (recv_ccount == 0) {
    // start bit
    if (0 == pb4) {
      recv_ccount = SUBSAMPLE;
    }
  } else {
    recv_ccount++;
    uint8_t bit_idx = (recv_ccount >> SUBSAMPLE_LN2);
    uint8_t cycle_idx = (recv_ccount & (SUBSAMPLE - 1));
    // Grab the second sample so that we know we're not on a transition.
    if ((SUBSAMPLE / 2) == cycle_idx) {
      // Ignore the stop and stop bits
      if (1 < bit_idx && bit_idx < 10) {
        recv_buf >>= 1;
        recv_buf |= (pb4 << 7);
        if (9 == bit_idx) {
          last_recv = recv_buf;
          new_last_recv = 1;
        }
      }
    } else if (10 == bit_idx) {
      recv_ccount = 0;
    }
  }
}

void send() {
  if (new_tx == 1 || tx_ccount != 0) {
    // This keeps the logic in line with listen()
    if (tx_ccount == 0) {
      tx_ccount = SUBSAMPLE;
    } else {
      tx_ccount++;
    }
    new_tx = 0;
    uint8_t bit_idx = (tx_ccount >> SUBSAMPLE_LN2);
    uint8_t cycle_idx = (tx_ccount & (SUBSAMPLE - 1));
    if (0 == cycle_idx) {
      if (1 == bit_idx) {
        PORTB &= ~(1 << PB3);
      } else if (1 < bit_idx && bit_idx < 10) {
        if (tx_buf & (1 << (bit_idx - 2))) {
          PORTB |= (1 << PB3);
        } else {
          PORTB &= ~(1 << PB3);
        }
      } else if (10 == bit_idx) {
        PORTB |= (1 << PB3);
      } else if (11 == bit_idx) {
        PORTB |= (1 << PB3);
        tx_ccount = 0;
      }
    }
  }
}

void setup_timer() {
  const uint32_t oc = F_CPU / 256 / SOFT_BAUD / SUBSAMPLE;
  _Static_assert(oc < 256, "SOFT_BAUD too high");
  _Static_assert(oc != 0, "SOFT_BAUD too low");
  OCR0A = (uint8_t)(oc & 0xff);
  TCNT0 = 0;
  // Wave Form Generation Mode 2: CTC
  // Toggle PD6 on compare match
  TCCR0A |= (1 << COM0A0) | (1 << WGM01);
  // prescaler = 256
  TCCR0B |= (1 << CS02);
}

void setup_uart() {
  // 8 because we're setting the 2x sample rate in UCSR0A
  const uint32_t ubrr0 = (F_CPU / 8L / UART_BAUD) - 1;
  // 12 because the atmega328p only uses the lower nibble of UBRR0H
  _Static_assert(ubrr0 < (1L << 12), "uart baud too high");
  _Static_assert(ubrr0 != 0, "uart baud too low");
  UBRR0L = (uint8_t) (ubrr0 & 0xFF);
  UBRR0H = (uint8_t) (ubrr0 >> 8);
  UCSR0A = (1 << U2X0);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  // 8 bit data
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

int main() {
  // LED on the Arduino, but also just a general debugging pin.
  DDRB |= (1 << PB5);
  // TX pin
  DDRB |= (1 << PB3);
  // For the timer output pin.
  // Used to test if the timer is working as expected.
  DDRD |= (1 << PD6);

  // Set UART Tx high.
  PORTB |= (1 << PB3);
  // Turn Off LED.
  PORTB &= ~(1 << PB5);

  setup_timer(SOFT_BAUD);
  setup_uart(UART_BAUD);

  while (1) {
    // Check if timer output compare was set.
    if (TIFR0 & (1 << OCF0A)) {
      // Clear the compare match flag.
      TIFR0 |= (1 << OCF0A);

      listen();
      send();

      if (new_next_to_tx && tx_ccount == 0) {
        new_next_to_tx = 0;
        new_tx = 1;
        tx_buf = next_to_tx;
      }
      if (new_last_recv) {
        new_last_recv = 0;
        _putc(last_recv);
        if (last_recv_packet_start) {
          last_recv_packet_start = 0;
          if (last_recv == 'N') {
            PORTB |= (1 << PB5);
          } else if (last_recv == 'T') {
            PORTB ^= (1 << PB5);
          } else {
            PORTB &= ~(1 << PB5);
          }
        } else {
          if (last_recv == '!') {
            last_recv_packet_start = 1;
          }
          new_next_to_tx = 1;
          next_to_tx = last_recv;
        }
      }
    }
  }
  return 0;
}
