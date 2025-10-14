// copyright AGPL3

#include "7b11b.h"
#include <stdio.h>

int main() {
  uint8_t msg = 42;
  uint16_t encoded = enc7b11b_encode_full(msg);

  printf("%d => ", msg);
  for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
    uint8_t t = (encoded & (1 << i)) ? 1 : 0;
    printf("%d", t);
  }
  printf("\n");

  const uint8_t noise[ENC7b11b_CODEWORD_LENGTH] = {1, 2, 3, 3, 3, 2, 1, 1, 3};

  enc7b11b_start_decode();
  for (uint8_t i = 0; i < ENC7b11b_CODEWORD_LENGTH; i++) {
    uint8_t t = (encoded & (1 << i)) ? 1 : 0;
    for (uint8_t j = 0; j < (ENC7b11b_SUBSAMPLES - noise[i]); j++) {
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
