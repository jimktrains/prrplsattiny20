#pragma once
// copyright AGPL3

#include <stdint.h>
// codeword_length=10
// max_run=4
// check_stop_bit=True
// check_start_bit=True
// check_parity=False
// print_raw_codeword=False
// parity='None'

#define ENC7b11b_CODEWORD_LENGTH 10
#define ENC7b11b_HAS_START_BIT 1
#define ENC7b11b_HAS_STOP_BIT 1

#define ENC7b11b_LN2_SUBSAMPLES 4
#define ENC7b11b_SUBSAMPLES (1 << ENC7b11b_LN2_SUBSAMPLES)
#define ENC7b11b_EXPECTED_SUBSAMPLE_COUNT                                      \
  (ENC7b11b_CODEWORD_LENGTH * ENC7b11b_SUBSAMPLES)
#define ENC7b11b_BALLOON 11
#define ENC7b11b_SUBSAMPLE_ALLOC_BITS                                          \
  ((ENC7b11b_BALLOON * ENC7b11b_EXPECTED_SUBSAMPLE_COUNT) / 10)
#define ENC7b11b_SUBSAMPLE_ALLOC_BYTES (1 + (ENC7b11b_SUBSAMPLE_ALLOC_BITS / 8))

// codeword_count=155
// code_rate=7:10

// balance stats
// zeros count
//     0     0
//     1     0
//     2     1
//     3    12 ***
//     4    40 **********
// #   5    52 *************
//     6    38 *********
//     7    12 ***
//     8     0
//     9     0
//    10     0

#define ENC7b11b_CODEWORD_COUNT 155
extern uint8_t enc7b11b_codewords[ENC7b11b_CODEWORD_COUNT];

uint8_t enc7b11b_encode(uint8_t b, uint8_t i);
uint16_t enc7b11b_encode_full(uint8_t b);
void enc7b11b_start_decode();
void enc7b11b_continue_decode(uint8_t v);
uint8_t enc7b11b_finish_decode();
