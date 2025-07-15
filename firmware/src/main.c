/*
 * Copyright 2025 jsk AGPL3
 *
 * File:   main.c
 * Author: jim
 *
 * Created on July 8, 2025, 1:19 AM
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#define SOFT_BAUD 300
#define RX_PIN PB0
#define TX_PIN PB1
#include "./soft_uart.h"


#define EP_A PA5
#define EP_B PA6
#define EP_C PB2
#define EP_CENTER PA4

#define EP_A_FLAG (1 << EP_A)
#define EP_B_FLAG (1 << EP_B)
#define EP_C_FLAG (1 << EP_C)
#define EP_CENTER_FLAG (1 << EP_CENTER)

#define EP_PORTA (EP_A_FLAG | EP_B_FLAG | EP_CENTER_FLAG)
#define EP_PORTB (EP_C_FLAG)

/*
 * LEDs are connected to PA5, PA6, and PB2.
 * Center LED is PA4 connected to ground at the cathode.
 * Use the internal pull up resistors to power them.
 */
#define STOP_1 0x65
#define STOP_2 0x25
#define APPR_1 0x62
#define APPR_2 0x52
#define CLEAR_1 0x26
#define CLEAR_2 0x56
#define CENTER 0x40
#define ALLOFF 0x00

#define STOP_1_BIT 0
#define STOP_2_BIT 1
#define APPR_1_BIT 2
#define APPR_2_BIT 3
#define CLEAR_1_BIT 4
#define CLEAR_2_BIT 5
#define CENTER_BIT 6

#define STOP_1_FLAG (1 << STOP_1_BIT)
#define STOP_2_FLAG (1 << STOP_2_BIT)
#define APPR_1_FLAG (1 << APPR_1_BIT)
#define APPR_2_FLAG (1 << APPR_2_BIT)
#define CLEAR_1_FLAG (1 << CLEAR_1_BIT)
#define CLEAR_2_FLAG (1 << CLEAR_2_BIT)
#define CENTER_FLAG (1 << CENTER_BIT)

#define STOP_ASP (STOP_1_FLAG | STOP_2_FLAG | CENTER_FLAG)
#define APPR_ASP (APPR_1_FLAG | APPR_2_FLAG | CENTER_FLAG)
#define CLEAR_ASP (CLEAR_1_FLAG | CLEAR_2_FLAG | CENTER_FLAG)

const uint8_t PROGMEM lightidx[7] = {STOP_1, STOP_2, APPR_1, APPR_2,
                                     CLEAR_1,  CLEAR_2,  CENTER};
uint8_t lightson = ALLOFF;
uint8_t cur_light_bit = 6;

void light_led(uint8_t light) {
  uint8_t source = 0x0f & light;
  uint8_t sink = (0xf0 & light) >> 4;

  // Put All LEDs into z state
  // i.e. input with no pull-up
  PUEA &= ~EP_PORTA;
  PUEB &= ~EP_PORTB;
  DDRA &= ~EP_PORTA;
  DDRB &= ~EP_PORTB;

  PUEB |= (source == 2) << 2;
  PUEA |= (source != 2 && source != 0) << source;

  DDRB |= (sink == 2) << 2;
  DDRA |= (sink != 2 && sink != 0) << sink;
}

int main(void) {
  // Init LEDs Off
  light_led(ALLOFF);
  // Set the LED pins to be a sink/ground when set as output.
  PORTA &= ~EP_PORTA;
  PORTB &= ~EP_PORTB;

  setup_timer(SOFT_BAUD);

  // Set DDR for Rx and Tx Pins
  DDRB |= ((1 << RX_PIN) | (1 << TX_PIN));

  while (1) {
    // Check if timer output compare was set.
    if (TIFR & (1 << OCF0A)) {
      // Clear the compare match flag.
      TIFR |= (1 << OCF0A);

      if (cur_light_bit == 6) {
        cur_light_bit = 0;
      } else {
        cur_light_bit += 1;
      }
      if (lightson & (1 << cur_light_bit)) {
        light_led(lightidx[cur_light_bit]);
      }

      listen();
      send();

      if (new_next_to_tx && tx_ccount == 0) {
        new_next_to_tx = 0;
        new_tx = 1;
        tx_buf = next_to_tx;
      }
      if (new_last_recv) {
        new_last_recv = 0;
        // Our command is right after the packet start, so we don't
        // retransmit it.
        if (last_recv_packet_start) {
          last_recv_packet_start = 0;
          if (last_recv == 'S') {
            lightson = STOP_ASP;
          } else if (last_recv == 'A') {
            lightson = APPR_ASP;
          } else if (last_recv == 'C') {
            lightson = CLEAR_ASP;
          } else if (last_recv >= 0x80) {
            lightson = last_recv;
          } else {
            lightson = ALLOFF;
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
}
