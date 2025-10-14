// copyright AGPL3

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "7b11b.h"

int main() {
  uint8_t fails = 0;
  for (uint16_t i = 0; i < 4096; i++) {
    uint8_t msg = 42;
    uint16_t encoded = enc7b11b_encode_full(msg);
    srand(time(NULL));

    printf("%d => ", msg);
    for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
      uint8_t t = (encoded & (1 << i)) ? 1 : 0;
      printf("%d", t);
    }
    printf("\n");

    int8_t noise[ENC7b11b_CODEWORD_LENGTH];
    printf("noise=");
    for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
      int8_t r = (rand() / ((RAND_MAX + 1u) / 4)) - 1;
      printf(" % 2d", r);
      noise[i] = r + ENC7b11b_SUBSAMPLES;
      ;
    }

    for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
    }
    printf("\n");

    printf("      ");
    for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
      printf(" % 2d", noise[i]);
    }
    printf("\n");
    printf("ENC7b11b_EXPECTED_SUBSAMPLE_COUNT=%d\n",
           ENC7b11b_EXPECTED_SUBSAMPLE_COUNT);
    printf("ENC7b11b_SUBSAMPLE_ALLOC_BITS=%d\n", ENC7b11b_SUBSAMPLE_ALLOC_BITS);

    for (uint8_t i = 0; i < ENC7b11b_SUBSAMPLE_ALLOC_BITS; i++) {
      if (i % 10 == 0) {
        printf("%d", i / 10);
      } else {
        printf(" ");
      }
    }
    printf("\n");
    for (uint8_t i = 0; i < ENC7b11b_SUBSAMPLE_ALLOC_BITS; i++) {
      printf("%d", i % 10);
    }
    printf("\n");

    enc7b11b_start_decode();
    for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
      uint8_t t = (encoded & (1 << i)) ? 1 : 0;
      for (uint8_t j = 0; j < noise[i]; j++) {
        printf("%d", t);
        enc7b11b_continue_decode(t);
      }
    }
    printf("\n");
    uint8_t decoded = enc7b11b_finish_decode();

    printf("decoded = %d ", decoded);
    for (uint8_t i = 0; i < 8; i++) {
      uint8_t t = (decoded & (1 << i)) ? 1 : 0;
      printf("%d", t);
    }
    printf("\n");

    if (decoded == msg) {
      printf("YAY!");
    } else {
      fails += 1;
      printf("BOOOOO :(");
      printf("BOOOOO :(");
      printf("BOOOOO :(");
    }
    printf("\n\n\n");
  }
  printf("fails=%d\n", fails);

  return 0;
}
