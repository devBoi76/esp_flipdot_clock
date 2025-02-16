#pragma once
// font: https://fontstruct.com/fontstructions/show/514877/9x5_matrix

#include <stdint.h>
#include <time.h>

#define CHAR_H 9
#define CHAR_GAP 1


typedef struct {
  uint8_t advance;
  uint8_t gap;
  uint8_t const * const data;
} font_char_t;


const font_char_t font_zero = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 1, 1,
    1, 0, 1, 0, 1,
    1, 1, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 1, 0,
    0, 1, 1, 0, 0,
  }
};

const font_char_t font_one = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_two = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 0,
  }
};

const font_char_t font_three = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_four = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 0, 0, 1, 0,
    0, 0, 1, 1, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 1, 0,
    1, 1, 1, 1, 1,
    0, 0, 0, 1, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 1, 0,
  }
};

const font_char_t font_five = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_six = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_seven = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
  }
};

const font_char_t font_eight = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_nine = {
  .advance = 5,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_colon = {
  .advance = 3,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 0, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, 0,
  }
};


const font_char_t font_space = {
  .advance = 3,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
  }
};

const font_char_t font_e = {
  .advance = 5,
  .gap = 1,
  .data = (const uint8_t[]) {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
  }
};
const font_char_t font_p = {
  .advance = 5,
  .gap = 1,
  .data = (const uint8_t[]) {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
  }
};

const font_char_t font_m = {
  .advance = 5,
  .gap = 1,
  .data = (const uint8_t[]) {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 1, 0, 1,
    1, 0, 1, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
  }
};

const font_char_t font_BIG_C = {
  .advance = 5,
  .gap = 1,
  .data = (const uint8_t[]) {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_BIG_A = {
  .advance = 6,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
  }
};

const font_char_t font_BIG_G = {
  .advance = 6,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    0, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0,
    1, 1, 0, 1, 1, 0,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 0,
  }
};

const font_char_t font_BIG_H = {
  .advance = 6,
  .gap = CHAR_GAP,
  .data = (const uint8_t[]){
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 1, 1,
  }
};

const font_char_t font_BIG_O = {
  .advance = 5,
  .gap = 1,
  .data = (const uint8_t[]) {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
  }
};

const font_char_t font_BIG_T = {
  .advance = 5,
  .gap = 1,
  .data = (const uint8_t[]) {
    1, 1, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
  }
};


const font_char_t font_bar = {
  .advance = 1,
  .gap = 0,
  .data = (const uint8_t[]) {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
  }
};

const font_char_t font_percent = {
  .advance = 5,
  .gap = 1,
  .data = (const uint8_t[]) {
    0, 1, 0, 0, 1,
    1, 0, 1, 0, 1,
    0, 1, 0, 0, 1,
    0, 0, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0,
    1, 0, 0, 1, 0,
    1, 0, 1, 0, 1,
    1, 0, 0, 1, 0,
  }
};

const font_char_t font_dot = {
  .advance = 2,
  .gap = 1,
  .data = (const uint8_t[]) {
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    1, 1,
    1, 1,
  }
};

const font_char_t font_null = {
  .advance = 0,
  .gap = 0,
  .data = (const uint8_t[]){},
};

// returns null if not found
const font_char_t* get_char_font(char c) {
  const font_char_t *ret = 0;
  switch(c) {
    // case '\0': ret = &font_null; break;
    case '0': ret = &font_zero; break;
    case '1': ret = &font_one; break;
    case '2': ret = &font_two; break;
    case '3': ret = &font_three; break;
    case '4': ret = &font_four; break;
    case '5': ret = &font_five; break;
    case '6': ret = &font_six; break;
    case '7': ret = &font_seven; break;
    case '8': ret = &font_eight; break;
    case '9': ret = &font_nine; break;
    case ':': ret = &font_colon; break;
    case 'e': ret = &font_e; break;
    case 'm': ret = &font_m; break;
    case 'p': ret = &font_p; break;
    case ' ': ret = &font_space; break;
    case 'A': ret = &font_BIG_A; break;
    case 'C': ret = &font_BIG_C; break;
    case 'G': ret = &font_BIG_G; break;
    case 'H': ret = &font_BIG_H; break;
    case 'O': ret = &font_BIG_O; break;
    case 'T': ret = &font_BIG_T; break;
    case '|': ret = &font_bar; break;
    case '%': ret = &font_percent; break;
    case '.': ret = &font_dot; break;
  }

  return ret;
}
