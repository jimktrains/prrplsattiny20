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

