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
uint8_t enc7b11b_codewords[ENC7b11b_CODEWORD_COUNT] = {
  0x55,  // i:000(0x00) D.x00 0b01010101 fullframe:0101010101
  0xd5,  // i:001(0x01) D.x01 0b11010101 fullframe:0101010111
  0x95,  // i:002(0x02) D.x02 0b10010101 fullframe:0101010011
  0x15,  // i:003(0x03) D.x03 0b00010101 fullframe:0101010001
  0xb5,  // i:004(0x04) D.x04 0b10110101 fullframe:0101011011
  0x35,  // i:005(0x05) D.x05 0b00110101 fullframe:0101011001
  0x75,  // i:006(0x06) D.x06 0b01110101 fullframe:0101011101
  0xa5,  // i:007(0x07) D.x07 0b10100101 fullframe:0101001011
  0x25,  // i:008(0x08) D.x08 0b00100101 fullframe:0101001001
  0x65,  // i:009(0x09) D.x09 0b01100101 fullframe:0101001101
  0xe5,  // i:010(0x0a) D.x0a 0b11100101 fullframe:0101001111
  0x45,  // i:011(0x0b) D.x0b 0b01000101 fullframe:0101000101
  0xc5,  // i:012(0x0c) D.x0c 0b11000101 fullframe:0101000111
  0x85,  // i:013(0x0d) D.x0d 0b10000101 fullframe:0101000011
  0xad,  // i:014(0x0e) D.x0e 0b10101101 fullframe:0101101011
  0x2d,  // i:015(0x0f) D.x0f 0b00101101 fullframe:0101101001
  0x6d,  // i:016(0x10) D.x10 0b01101101 fullframe:0101101101
  0xed,  // i:017(0x11) D.x11 0b11101101 fullframe:0101101111
  0x4d,  // i:018(0x12) D.x12 0b01001101 fullframe:0101100101
  0xcd,  // i:019(0x13) D.x13 0b11001101 fullframe:0101100111
  0x8d,  // i:020(0x14) D.x14 0b10001101 fullframe:0101100011
  0x0d,  // i:021(0x15) D.x15 0b00001101 fullframe:0101100001
  0x5d,  // i:022(0x16) D.x16 0b01011101 fullframe:0101110101
  0xdd,  // i:023(0x17) D.x17 0b11011101 fullframe:0101110111
  0x9d,  // i:024(0x18) D.x18 0b10011101 fullframe:0101110011
  0x1d,  // i:025(0x19) D.x19 0b00011101 fullframe:0101110001
  0xbd,  // i:026(0x1a) D.x1a 0b10111101 fullframe:0101111011
  0x3d,  // i:027(0x1b) D.x1b 0b00111101 fullframe:0101111001
  0xa9,  // i:028(0x1c) D.x1c 0b10101001 fullframe:0100101011
  0x29,  // i:029(0x1d) D.x1d 0b00101001 fullframe:0100101001
  0x69,  // i:030(0x1e) D.x1e 0b01101001 fullframe:0100101101
  0xe9,  // i:031(0x1f) D.x1f 0b11101001 fullframe:0100101111
  0x49,  // i:032(0x20) D.x20 0b01001001 fullframe:0100100101
  0xc9,  // i:033(0x21) D.x21 0b11001001 fullframe:0100100111
  0x89,  // i:034(0x22) D.x22 0b10001001 fullframe:0100100011
  0x09,  // i:035(0x23) D.x23 0b00001001 fullframe:0100100001
  0x59,  // i:036(0x24) D.x24 0b01011001 fullframe:0100110101
  0xd9,  // i:037(0x25) D.x25 0b11011001 fullframe:0100110111
  0x99,  // i:038(0x26) D.x26 0b10011001 fullframe:0100110011
  0x19,  // i:039(0x27) D.x27 0b00011001 fullframe:0100110001
  0xb9,  // i:040(0x28) D.x28 0b10111001 fullframe:0100111011
  0x39,  // i:041(0x29) D.x29 0b00111001 fullframe:0100111001
  0x79,  // i:042(0x2a) D.x2a 0b01111001 fullframe:0100111101
  0x51,  // i:043(0x2b) D.x2b 0b01010001 fullframe:0100010101
  0xd1,  // i:044(0x2c) D.x2c 0b11010001 fullframe:0100010111
  0x91,  // i:045(0x2d) D.x2d 0b10010001 fullframe:0100010011
  0x11,  // i:046(0x2e) D.x2e 0b00010001 fullframe:0100010001
  0xb1,  // i:047(0x2f) D.x2f 0b10110001 fullframe:0100011011
  0x31,  // i:048(0x30) D.x30 0b00110001 fullframe:0100011001
  0x71,  // i:049(0x31) D.x31 0b01110001 fullframe:0100011101
  0xa1,  // i:050(0x32) D.x32 0b10100001 fullframe:0100001011
  0x21,  // i:051(0x33) D.x33 0b00100001 fullframe:0100001001
  0x61,  // i:052(0x34) D.x34 0b01100001 fullframe:0100001101
  0xe1,  // i:053(0x35) D.x35 0b11100001 fullframe:0100001111
  0xab,  // i:054(0x36) D.x36 0b10101011 fullframe:0110101011
  0x2b,  // i:055(0x37) D.x37 0b00101011 fullframe:0110101001
  0x6b,  // i:056(0x38) D.x38 0b01101011 fullframe:0110101101
  0xeb,  // i:057(0x39) D.x39 0b11101011 fullframe:0110101111
  0x4b,  // i:058(0x3a) D.x3a 0b01001011 fullframe:0110100101
  0xcb,  // i:059(0x3b) D.x3b 0b11001011 fullframe:0110100111
  0x8b,  // i:060(0x3c) D.x3c 0b10001011 fullframe:0110100011
  0x0b,  // i:061(0x3d) D.x3d 0b00001011 fullframe:0110100001
  0x5b,  // i:062(0x3e) D.x3e 0b01011011 fullframe:0110110101
  0xdb,  // i:063(0x3f) D.x3f 0b11011011 fullframe:0110110111
  0x9b,  // i:064(0x40) D.x40 0b10011011 fullframe:0110110011
  0x1b,  // i:065(0x41) D.x41 0b00011011 fullframe:0110110001
  0xbb,  // i:066(0x42) D.x42 0b10111011 fullframe:0110111011
  0x3b,  // i:067(0x43) D.x43 0b00111011 fullframe:0110111001
  0x7b,  // i:068(0x44) D.x44 0b01111011 fullframe:0110111101
  0x53,  // i:069(0x45) D.x45 0b01010011 fullframe:0110010101
  0xd3,  // i:070(0x46) D.x46 0b11010011 fullframe:0110010111
  0x93,  // i:071(0x47) D.x47 0b10010011 fullframe:0110010011
  0x13,  // i:072(0x48) D.x48 0b00010011 fullframe:0110010001
  0xb3,  // i:073(0x49) D.x49 0b10110011 fullframe:0110011011
  0x73,  // i:074(0x4a) D.x4a 0b01110011 fullframe:0110011101
  0xa3,  // i:075(0x4b) D.x4b 0b10100011 fullframe:0110001011
  0x23,  // i:076(0x4c) D.x4c 0b00100011 fullframe:0110001001
  0x63,  // i:077(0x4d) D.x4d 0b01100011 fullframe:0110001101
  0x43,  // i:078(0x4e) D.x4e 0b01000011 fullframe:0110000101
  0x57,  // i:079(0x4f) D.x4f 0b01010111 fullframe:0111010101
  0xd7,  // i:080(0x50) D.x50 0b11010111 fullframe:0111010111
  0x97,  // i:081(0x51) D.x51 0b10010111 fullframe:0111010011
  0x17,  // i:082(0x52) D.x52 0b00010111 fullframe:0111010001
  0xb7,  // i:083(0x53) D.x53 0b10110111 fullframe:0111011011
  0x37,  // i:084(0x54) D.x54 0b00110111 fullframe:0111011001
  0x77,  // i:085(0x55) D.x55 0b01110111 fullframe:0111011101
  0xa7,  // i:086(0x56) D.x56 0b10100111 fullframe:0111001011
  0x27,  // i:087(0x57) D.x57 0b00100111 fullframe:0111001001
  0x67,  // i:088(0x58) D.x58 0b01100111 fullframe:0111001101
  0x47,  // i:089(0x59) D.x59 0b01000111 fullframe:0111000101
  0xaf,  // i:090(0x5a) D.x5a 0b10101111 fullframe:0111101011
  0x2f,  // i:091(0x5b) D.x5b 0b00101111 fullframe:0111101001
  0x6f,  // i:092(0x5c) D.x5c 0b01101111 fullframe:0111101101
  0xef,  // i:093(0x5d) D.x5d 0b11101111 fullframe:0111101111
  0x4f,  // i:094(0x5e) D.x5e 0b01001111 fullframe:0111100101
  0xaa,  // i:095(0x5f) D.x5f 0b10101010 fullframe:0010101011
  0x2a,  // i:096(0x60) D.x60 0b00101010 fullframe:0010101001
  0x6a,  // i:097(0x61) D.x61 0b01101010 fullframe:0010101101
  0xea,  // i:098(0x62) D.x62 0b11101010 fullframe:0010101111
  0x4a,  // i:099(0x63) D.x63 0b01001010 fullframe:0010100101
  0xca,  // i:100(0x64) D.x64 0b11001010 fullframe:0010100111
  0x8a,  // i:101(0x65) D.x65 0b10001010 fullframe:0010100011
  0x0a,  // i:102(0x66) D.x66 0b00001010 fullframe:0010100001
  0x5a,  // i:103(0x67) D.x67 0b01011010 fullframe:0010110101
  0xda,  // i:104(0x68) D.x68 0b11011010 fullframe:0010110111
  0x9a,  // i:105(0x69) D.x69 0b10011010 fullframe:0010110011
  0x1a,  // i:106(0x6a) D.x6a 0b00011010 fullframe:0010110001
  0xba,  // i:107(0x6b) D.x6b 0b10111010 fullframe:0010111011
  0x3a,  // i:108(0x6c) D.x6c 0b00111010 fullframe:0010111001
  0x7a,  // i:109(0x6d) D.x6d 0b01111010 fullframe:0010111101
  0x52,  // i:110(0x6e) D.x6e 0b01010010 fullframe:0010010101
  0xd2,  // i:111(0x6f) D.x6f 0b11010010 fullframe:0010010111
  0x92,  // i:112(0x70) D.x70 0b10010010 fullframe:0010010011
  0x12,  // i:113(0x71) D.x71 0b00010010 fullframe:0010010001
  0xb2,  // i:114(0x72) D.x72 0b10110010 fullframe:0010011011
  0x72,  // i:115(0x73) D.x73 0b01110010 fullframe:0010011101
  0xa2,  // i:116(0x74) D.x74 0b10100010 fullframe:0010001011
  0x22,  // i:117(0x75) D.x75 0b00100010 fullframe:0010001001
  0x62,  // i:118(0x76) D.x76 0b01100010 fullframe:0010001101
  0x42,  // i:119(0x77) D.x77 0b01000010 fullframe:0010000101
  0x56,  // i:120(0x78) D.x78 0b01010110 fullframe:0011010101
  0xd6,  // i:121(0x79) D.x79 0b11010110 fullframe:0011010111
  0x96,  // i:122(0x7a) D.x7a 0b10010110 fullframe:0011010011
  0x16,  // i:123(0x7b) D.x7b 0b00010110 fullframe:0011010001
  0xb6,  // i:124(0x7c) D.x7c 0b10110110 fullframe:0011011011
  0x76,  // i:125(0x7d) D.x7d 0b01110110 fullframe:0011011101
  0xa6,  // i:126(0x7e) D.x7e 0b10100110 fullframe:0011001011
  0x46,  // i:127(0x7f) D.x7f 0b01000110 fullframe:0011000101
  0xae,  // i:128(0x80) K.x00 0b10101110 fullframe:0011101011
  0x2e,  // i:129(0x81) K.x01 0b00101110 fullframe:0011101001
  0x6e,  // i:130(0x82) K.x02 0b01101110 fullframe:0011101101
  0x4e,  // i:131(0x83) K.x03 0b01001110 fullframe:0011100101
  0x5e,  // i:132(0x84) K.x04 0b01011110 fullframe:0011110101
  0x54,  // i:133(0x85) K.x05 0b01010100 fullframe:0001010101
  0xd4,  // i:134(0x86) K.x06 0b11010100 fullframe:0001010111
  0x94,  // i:135(0x87) K.x07 0b10010100 fullframe:0001010011
  0x14,  // i:136(0x88) K.x08 0b00010100 fullframe:0001010001
  0xb4,  // i:137(0x89) K.x09 0b10110100 fullframe:0001011011
  0x34,  // i:138(0x8a) K.x0a 0b00110100 fullframe:0001011001
  0x74,  // i:139(0x8b) K.x0b 0b01110100 fullframe:0001011101
  0xa4,  // i:140(0x8c) K.x0c 0b10100100 fullframe:0001001011
  0x24,  // i:141(0x8d) K.x0d 0b00100100 fullframe:0001001001
  0x64,  // i:142(0x8e) K.x0e 0b01100100 fullframe:0001001101
  0x44,  // i:143(0x8f) K.x0f 0b01000100 fullframe:0001000101
  0xac,  // i:144(0x90) K.x10 0b10101100 fullframe:0001101011
  0x2c,  // i:145(0x91) K.x11 0b00101100 fullframe:0001101001
  0x6c,  // i:146(0x92) K.x12 0b01101100 fullframe:0001101101
  0x4c,  // i:147(0x93) K.x13 0b01001100 fullframe:0001100101
  0x5c,  // i:148(0x94) K.x14 0b01011100 fullframe:0001110101
  0xa8,  // i:149(0x95) K.x15 0b10101000 fullframe:0000101011
  0x28,  // i:150(0x96) K.x16 0b00101000 fullframe:0000101001
  0x68,  // i:151(0x97) K.x17 0b01101000 fullframe:0000101101
  0xe8,  // i:152(0x98) K.x18 0b11101000 fullframe:0000101111
  0x48,  // i:153(0x99) K.x19 0b01001000 fullframe:0000100101
  0x58,  // i:154(0x9a) K.x1a 0b01011000 fullframe:0000110101
};
