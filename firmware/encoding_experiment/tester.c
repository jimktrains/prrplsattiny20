// copyright AGPL3
#include <stdio.h>
#include "7b11b.h"


int main() {
  uint8_t msg = 42;
  uint16_t encoded = enc7b11b_encode_full(msg);

  printf("%d => ", msg);
  for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
    uint8_t t = (encoded & (1 << i)) ? 1 : 0;
    printf("%d", t);
  }
  printf("\n");

  enc7b11b_start_decode();
  for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
    uint8_t t = (encoded & (1 << i)) ? 1 : 0;
    for (uint8_t j = 0; j < ENC7b11b_SUBSAMPLES; j++) {
      enc7b11b_continue_decode(t);
    }
  }
  uint8_t decoded = enc7b11b_finish_decode();

  printf("decoded = %d ", decoded);
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t t = (decoded & (1 << i)) ? 1 : 0;
    printf("%d", t);
  }
  printf("\n");


  return 0;
}
