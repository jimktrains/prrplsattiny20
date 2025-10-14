// copyright AGPL3

#include <stdio.h>
#include <string.h>

#include "7b11b.h"
#include "7b11bcw.h"

uint8_t enc7b11b_encode(uint8_t b, uint8_t i) {
  // start bit
  if (i == 0 && ENC7b11b_HAS_START_BIT) {
    return 0;
  } else if (i == 9 && ENC7b11b_HAS_STOP_BIT) {
    return 1;
  } else {
    uint8_t cw = enc7b11b_codewords[b];
    return (cw >> (i - ENC7b11b_HAS_START_BIT)) & 0x1;
  }
}

uint16_t enc7b11b_encode_full(uint8_t b) {
  uint16_t t = 0;
  for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
    t |= enc7b11b_encode(b, i) << i;
  }
  return t;
}

uint8_t enc7b11b_subsamples[ENC7b11b_SUBSAMPLE_ALLOC_BYTES];
uint8_t enc7b11b_started = 0;
uint8_t enc7b11b_subsample = 0;

void enc7b11b_start_decode() {
  enc7b11b_started = 0;
  enc7b11b_subsample = 0;
}

void enc7b11b_continue_decode(uint8_t v) {
  uint8_t bit = enc7b11b_subsample & 0x7;
  uint8_t byte = enc7b11b_subsample >> 3;
  uint8_t t = v == 0 ? 0 : 1;

  if (enc7b11b_subsample >= ENC7b11b_SUBSAMPLE_ALLOC_BITS) {
    return;
  }

  if ((t == 0) && (enc7b11b_started == 0)) {
    enc7b11b_started = 1;
  }

  if (enc7b11b_started) {
    enc7b11b_subsamples[byte] |= t << bit;
    enc7b11b_subsample += 1;
  }
}

uint8_t enc7b11b_finish_decode() {
  uint8_t shortest_run = 255;
  uint8_t current_run = 0;
  uint8_t runs[ENC7b11b_CODEWORD_LENGTH];
  uint8_t runs_v[ENC7b11b_CODEWORD_LENGTH];
  uint8_t run_cnt = 0;
  uint8_t last_v = 1;
  uint16_t fullcodeword = 0;
  uint8_t shortcodeword;
  uint8_t total_samples = 0;

  memset(runs, 0, sizeof(runs));
  memset(runs_v, 0, sizeof(runs_v));

  for (uint8_t i = 0; i < ENC7b11b_SUBSAMPLE_ALLOC_BITS; i++) {
    uint8_t bit = i & 0x7;
    uint8_t byte = i >> 3;
    uint8_t this_v = (enc7b11b_subsamples[byte] & (1 << bit)) ? 1 : 0;

    if (this_v != last_v) {
      printf("%d,%d ", current_run, shortest_run);
      if (current_run != 0) {
        runs_v[run_cnt] = last_v;
        runs[run_cnt] = current_run;
        total_samples += current_run;
        if (current_run < shortest_run) {
          shortest_run = current_run;
        }
        run_cnt += 1;
      }
      current_run = 0;
    }
    last_v = this_v;
    current_run += 1;
  }
  printf("\n");

  printf("shortest_run=%d\n", shortest_run);
  uint8_t average_samples_per_bit = 10*total_samples / ENC7b11b_CODEWORD_LENGTH;
  printf("samples/bit=10*%d/%d=%d\n", total_samples, ENC7b11b_CODEWORD_LENGTH,
         average_samples_per_bit);
  for (uint8_t i = 0, j = 0; i < run_cnt; i++) {
    uint8_t bits_in_run = ((10 * runs[i]) + (average_samples_per_bit/2) + 1) / average_samples_per_bit;
    if (bits_in_run > ENC7b11b_MAX_RUN) {
      bits_in_run = ENC7b11b_MAX_RUN;
    } else if (bits_in_run < 1) {
      bits_in_run = 1;
    }
    for (uint8_t k = 0; k < bits_in_run; k++, j++) {
      fullcodeword |= (runs_v[i] << j);
    }
    printf("%d: %d => %d of %d \n", i, runs[i], bits_in_run, runs_v[i]);
  }
  shortcodeword = (fullcodeword >> 1) & 0xff;

  printf("fullcw=");
  for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
    uint8_t t = (fullcodeword & (1 << i)) ? 1 : 0;
    printf("%d", t);
  }
  printf("\nshortcw=");
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t t = (shortcodeword & (1 << i)) ? 1 : 0;
    printf("%d", t);
  }
  printf("\n");

  for (uint8_t i = 0; i < ENC7b11b_CODEWORD_COUNT; i++) {
    if (enc7b11b_codewords[i] == shortcodeword) {
      return i;
    }
  }

  return 255;
}
