#pragma once

#include <stdint.h>
#include <time.h>

#define CHAR_H 9
#define CHAR_GAP 1

typedef struct {
  uint8_t advance;
  uint8_t const * const data;
} font_char_t;


const font_char_t font_zero = {
  .advance = 5,
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

const font_char_t font_BIG_A = {
  .advance = 6,
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

const font_char_t font_bar = {
  .advance = 1,
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

// returns null if not found
const font_char_t* get_char_font(char c) {
  const font_char_t *ret = 0;
  switch(c) {
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
    case ' ': ret = &font_space; break;
    case 'A': ret = &font_BIG_A; break;
    case 'G': ret = &font_BIG_G; break;
    case 'H': ret = &font_BIG_H; break;
    case '|': ret = &font_bar; break;
  }

  return ret;
}
