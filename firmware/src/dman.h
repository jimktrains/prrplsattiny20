/*
 * Copyright 2025 jsk AGPL3
 */
#pragma once

#include <stdint.h>
#include <avr/io.h>

#define SUBSAMPLE_LN2   2
#define SUBSAMPLE   (1 << SUBSAMPLE_LN2)

uint8_t rx_ccount = 0;
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

uint8_t last_seen = 1;
uint8_t tick_since_last_trans = 0;
uint8_t zero_was_last = 0;
uint8_t first_in_tx = 1;

#define TX_SEND_0 (PORTB &= ~(1 << TX_PIN));
#define TX_SEND_1 (PORTB |= (1 << TX_PIN));
#define TX_TOGGLE (PORTB ^= (1 << TX_PIN));

void listen() {
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_1;
  uint8_t pb4 = (PINB & (1 << RX_PIN)) ? 1 : 0;
  // TX_TOGGLE;
  if ((tick_since_last_trans > (3*SUBSAMPLE)) && first_in_tx == 0) {
    last_recv = recv_buf >> 4;
    new_last_recv = 1;
    recv_buf = 0;
    zero_was_last = 0;
    first_in_tx = 1;
    rx_ccount = 0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
  }
  if (last_seen == pb4) {
    tick_since_last_trans++;
  } else {
    if (first_in_tx) {
      first_in_tx = 0;
      tick_since_last_trans = 0;
    } else {
      if (tick_since_last_trans < (3*SUBSAMPLE)) {
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_0;
        // for (int i = 0; i < tick_since_last_trans; i++) {
        //   TX_SEND_0;
        //   TX_SEND_1;
        //   TX_SEND_0;
        // }
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_0;
        // TX_SEND_1;
        if (zero_was_last) {
          zero_was_last = 0;
        } else {
          if (rx_ccount < 4) {
            rx_ccount++;
            recv_buf >>= 1;
            zero_was_last = 1;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_0;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_1;
            // TX_SEND_0;
            if (tick_since_last_trans >= (SUBSAMPLE + (SUBSAMPLE/2)  )) {
              recv_buf |= 0x80;
              zero_was_last = 0;
              // TX_SEND_0;
              // TX_SEND_1;
              // TX_SEND_1;
              // TX_SEND_1;
              // TX_SEND_1;
              // TX_SEND_1;
              // TX_SEND_0;
            } else {
            }
            TX_SEND_1;
          }
        }
      } else {
        rx_ccount = 0;
      }
    }
    tick_since_last_trans = 0;
  }
  last_seen = pb4;
}

void send() {
  if (new_tx || (tx_ccount != 0)) {
    new_tx = 0;
    uint8_t bit_idx = tx_ccount >> (SUBSAMPLE_LN2 + 1);
    uint8_t tx_phase = (tx_ccount >> SUBSAMPLE_LN2) & 0x1;
    uint8_t tx_subsample = tx_ccount & (SUBSAMPLE - 1);
    /* uart    bit   phase
     * start    0      0
     * 0        0      1
     * 1        1      0
     * 2        1      1
     * 3        2      0
     * 4        2      1
     * 5        3      0
     * 6        3      1
     * 7        4      0
     * end      4      1
     */

    // TX_SEND_0;
    // TX_SEND_1;
    // TX_SEND_1;
    // TX_SEND_0;

    // for (int i = 0; i < bit_idx; i++) {
    //   TX_SEND_0;
    //   TX_SEND_1;
    //   TX_SEND_0;
    // }

    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;

    // for (int i = 0; i < tx_phase; i++) {
    //   TX_SEND_0;
    //   TX_SEND_1;
    //   TX_SEND_0;
    // }

    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;

    // for (int i = 0; i < tx_subsample; i++) {
    //   TX_SEND_0;
    //   TX_SEND_1;
    //   TX_SEND_0;
    // }

    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;
    //   TX_SEND_0;

    // if (tx_buf & 0x01) {
    //   TX_SEND_0;
    //   TX_SEND_1;
    //   TX_SEND_0;
    // }


    // TX_SEND_0;
    // TX_SEND_1;
    // TX_SEND_1;
    // TX_SEND_1;
    // TX_SEND_1;
    // TX_SEND_0;

    tx_ccount++;
    if (bit_idx == 4 && tx_phase == 1) {
      TX_SEND_1;
      tx_ccount = 0;
      tx_buf = 0;
    } else {
      if (tx_subsample == 0) {
        if (tx_phase == 0) {
         TX_TOGGLE;
        } else {
          if ((tx_buf & 0x01) == 0) {
           TX_TOGGLE;
          }
          tx_buf >>= 1;
        }
      }
    }
  }
}

void setup_timer() {
  const uint32_t oc = (F_CPU / 64 / SOFT_BAUD / SUBSAMPLE);
  _Static_assert(oc < 256, "SOFT_BAUD too high");
  _Static_assert(oc != 0, "SOFT_BAUD too low");
  OCR0A = (uint8_t)(oc & 0xff);
  TCNT0 = 0;
  // Wave Form Generation Mode 2: CTC (WGM01=1)
  TCCR0A |= (1 << WGM01);
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
