/*
 * Copyright 2025 jsk AGPL3
 */
#pragma once

#include <stdint.h>
#include <avr/io.h>

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


void listen() {
  uint8_t pb4 = (PINB & (1 << RX_PIN)) ? 1 : 0;

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
        PORTB &= ~(1 << TX_PIN);
      } else if (1 < bit_idx && bit_idx < 10) {
        if (tx_buf & (1 << (bit_idx - 2))) {
          PORTB |= (1 << TX_PIN);
        } else {
          PORTB &= ~(1 << TX_PIN);
        }
      } else if (10 == bit_idx) {
        PORTB |= (1 << TX_PIN);
      } else if (11 == bit_idx) {
        PORTB |= (1 << TX_PIN);
        tx_ccount = 0;
      }
    }
  }
}

void setup_timer() {
  // To be honest, it took me a long while to figure out why I had to use a
  // constant of 532 when the clock frequency is 8MHz to get the timing right.
  // It was derived via experimentation. I want a 1200Hz timer (300 baud * quad
  // subsampling), the clock should be 8MHz from the datasheet, and I'm setting
  // the divisor to 64, so it should be 8000000 / 64 / 300 / 4.
  //
  // Here's what I would expect:
  //
  //   1sec/1200symbols
  //     .00083333333333333333 sec / symbol
  //   (8000000 cycles / sec) / (64 cycles / tick) / (1200 symbols / sec)
  //     104.16666666666666666666 ticks / symbol
  //   (104 ticks / symbol) * (64 cycles / tick) / (8000000 cycles / sec)
  //     .00083200000000000000 sec / symbol
  //
  // but, if we do the math as is:
  //
  //   (8000000 cycles / sec) / (532 cycles / tick) / (1200 symbols / sec)
  //     12.53132832080200501253 ticks / symbol
  //
  // If we take that number, use the clock divisor I'm setting, and the
  // toggle time of a pin toggled on that timer, then
  //
  //   12 (ticks / symbol) * (64 cycles / tick) / (.00084 sec / symbol)
  //     914285.71428571428571428571 cycles / sec
  //
  // which is just under 1MHz. But the chip has an 8MHz internal oscilator
  // according to the datasheet, so why is it running at 1/8th that speed?
  //
  // After rereading the datasheet, I noticed §6.3 "System Clock Prescaler".
  // Then, following that to §6.5.2 "CLKPSR – Clock Prescale Register" I saw
  // it -- a default clock division factor of 8.
  //
  // RTFM kids.

  const uint32_t oc = F_CPU / 64 / SOFT_BAUD / SUBSAMPLE;
  _Static_assert(oc < 256, "SOFT_BAUD too high");
  _Static_assert(oc != 0, "SOFT_BAUD too low");
  OCR0A = (uint8_t)(oc & 0xff);
  TCNT0 = 0;
  // Wave Form Generation Mode 2: CTC ( COM0A0=1 COM0A1=0)
  // Toggle OC0A/PB2 on compare match (WGM01=1)
  TCCR0A |= (1 << COM0A0) | (0 << WGM01);
  // CS02 CS01 CS00 Description
  // 0    0    0 No clock source (Timer/Counter stopped)
  // 0    0    1 clkI/O/(No prescaling)
  // 0    1    0 clkI/O/8 (From prescaler)
  // 0    1    1 clkI/O/64 (From prescaler)
  // 1    0    0 clkI/O/256 (From prescaler)
  // 1    0    1 clkI/O/1024 (From prescaler)
  TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
}

uint8_t timer_tick() {
    // Check if timer output compare was set.
    if (TIFR & (1 << OCF0A)) {
      // Clear the compare match flag.
      TIFR |= (1 << OCF0A);
      return 1;
    }
    return 0;
}
