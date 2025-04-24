#include <Arduino.h>
#include "flipdot_interface.hpp"
#include "util.h"

#define CLK_PIN 25
#define DATA_PIN 26
#define LOAD_PIN 15
#define EN_PIN 16

screen_cursor_t bad_pixels;
screen_cursor_t last_screen;

uint8_t fp2800_addr_to_number(uint8_t digit, uint8_t segment);

uint8_t x_to_fp_addr(uint8_t x) {
  switch(x){
    case 27: return fp2800_addr_to_number(0, 'A');
    case 26: return fp2800_addr_to_number(0, 'B');
    case 25: return fp2800_addr_to_number(0, 'C');
    case 24: return fp2800_addr_to_number(0, 'D');
    case 23: return fp2800_addr_to_number(0, 'F');
    case 22: return fp2800_addr_to_number(0, 'E');
    case 21: return fp2800_addr_to_number(0, 'G');
    case 20: return fp2800_addr_to_number(3, 'A');
    case 19: return fp2800_addr_to_number(3, 'B');
    case 18: return fp2800_addr_to_number(3, 'C');
    case 17: return fp2800_addr_to_number(3, 'D');
    case 16: return fp2800_addr_to_number(3, 'F');
    case 15: return fp2800_addr_to_number(3, 'E');
    case 14: return fp2800_addr_to_number(3, 'G');
    case 13: return fp2800_addr_to_number(2, 'G');
    case 12: return fp2800_addr_to_number(2, 'E');
    case 11: return fp2800_addr_to_number(2, 'A');
    case 10: return fp2800_addr_to_number(2, 'B');
    case 9: return fp2800_addr_to_number(2, 'F');
    case 8: return fp2800_addr_to_number(2, 'C');
    case 7: return fp2800_addr_to_number(2, 'D');
    case 6: return fp2800_addr_to_number(1, 'G');
    case 5: return fp2800_addr_to_number(1, 'E');
    case 4: return fp2800_addr_to_number(1, 'A');
    case 3: return fp2800_addr_to_number(1, 'B');
    case 2: return fp2800_addr_to_number(1, 'C');
    case 1: return fp2800_addr_to_number(1, 'F');
    case 0: return fp2800_addr_to_number(1, 'D');
  }
}

uint8_t y_to_fp_addr(uint8_t y, bool state) {
  switch(y){
    case 8: return fp2800_addr_to_number(3, state ? 'E' : 'F');
    case 7: return fp2800_addr_to_number(state ? 1 : 2, 'A');
    case 6: return fp2800_addr_to_number(state ? 1 : 2, 'B');
    case 5: return fp2800_addr_to_number(state ? 1 : 2, 'C');
    case 4: return fp2800_addr_to_number(state ? 1 : 2, 'D');
    case 3: return fp2800_addr_to_number(state ? 1 : 2, 'E');
    case 2: return fp2800_addr_to_number(state ? 1 : 2, 'F');
    case 1: return fp2800_addr_to_number(state ? 1 : 2, 'G');
    case 0: return fp2800_addr_to_number(3, state ? 'A' : 'B');
  }
}
void flipdot_init() {
    pinMode(LOAD_PIN, OUTPUT);
    digitalWrite(LOAD_PIN, HIGH);
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, HIGH);
    pinMode(DATA_PIN, OUTPUT);
    digitalWrite(DATA_PIN, HIGH);
    pinMode(CLK_PIN, OUTPUT);
    digitalWrite(CLK_PIN, HIGH);

    init_cursor(&bad_pixels);
    // bad_pixels.scr[8 * SCREEN_W + 0] = 1;
    // bad_pixels.scr[3 * SCREEN_W + 2] = 1;
    // bad_pixels.scr[0 * SCREEN_W + 6] = 1;
    // bad_pixels.scr[1 * SCREEN_W + 13] = 1;
    // bad_pixels.scr[6 * SCREEN_W + 16] = 1;
    // bad_pixels.scr[4 * SCREEN_W + 20] = 1;
    // bad_pixels.scr[3 * SCREEN_W + 20] = 1;
    // bad_pixels.scr[2 * SCREEN_W + 20] = 1;
    // bad_pixels.scr[3 * SCREEN_W + 21] = 1;
  
    init_cursor(&last_screen);
}

void flipdot_pulse_clk() {
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(50);
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(50);
}
void flipdot_write_byte(uint8_t data) {
  //Serial.print("WRITE_BYTE: ");
  //Serial.print(data);
  //Serial.print("\t");

  for (size_t i = 0; i < 8; i += 1) {
    
    // invert
    digitalWrite(DATA_PIN, !(data & 0b10000000));
    // Serial.print((data & 0b10000000) != 0);
     data = data << 1;

    //digitalWrite(DATA_PIN, !(data & 0b1));
    //Serial.print((data & 0b1) != 0);
    //data = data >> 1;
    
    flipdot_pulse_clk();
  }
  // Serial.println("");
}

#define DATA_HIGH (0b01000000)
#define DATA_LOW (0b00000000)
#define ADDR_MASK (0b00011111)

// can take either number or letter for the segment
uint8_t fp2800_addr_to_number(uint8_t digit, uint8_t segment) {
  if (segment >= 'a' && segment <= 'z') {
    segment = segment - 'a' + 1;
  } else if (segment >= 'A' && segment <= 'Z') {
    segment = segment - 'A' + 1;
  }
  return ((digit & 0b11) << 3) | (segment & 0b111);

}
uint8_t flipdot_get_word(uint8_t chip_addr, bool data) {
  return ((chip_addr & ADDR_MASK) | (data ? DATA_HIGH : DATA_LOW));
}

void flipdot_set_pixel(uint8_t addr_x, uint8_t addr_y, bool state) {
  flipdot_write_byte(flipdot_get_word(addr_x, !state));
  flipdot_write_byte(flipdot_get_word(addr_y, state));
  
  digitalWrite(LOAD_PIN, LOW);
  delayMicroseconds(50);
  digitalWrite(LOAD_PIN, HIGH);
  delayMicroseconds(50);
  
  digitalWrite(EN_PIN, LOW);
  delayMicroseconds(1000);
  digitalWrite(EN_PIN, HIGH);
  delayMicroseconds(1000);
}

void flipdot_set_pixel_xy(uint8_t x, uint8_t y, bool state) {
  bool is_bad_pixel = bad_pixels.scr[y * SCREEN_W + x] == 1;
  bool corrected_state = state ^ is_bad_pixel;
  uint8_t y_addr = y_to_fp_addr(y, corrected_state);
  uint8_t x_addr = x_to_fp_addr(x);
  flipdot_set_pixel(x_addr, y_addr, corrected_state);
}

void flipdot_write_screen(screen_cursor_t *cur) {
  // for (int D = 1; D >= 0; D -= 1) {
  //   for (int y = 0; y < SCREEN_H; y += 1) {
  //     for (int x = 0; x < SCREEN_W; x += 1) {
  //       flipdot_set_pixel_xy(x, y, D);
  //     }
  //   }
  // }

  for (int y = 0; y < SCREEN_H; y += 1) {
    for (int x = 0; x < SCREEN_W; x += 1) {
      size_t screen_coords = y*SCREEN_W+x;
      bool target_state = false;
      if (cur->scr[screen_coords] & cur->scr_mask[screen_coords] == 1)
        target_state = true;
      else if (cur->scr[screen_coords] == 1 && cur->scr_mask[screen_coords] == 0)
        target_state = false; // no mask
      else
        target_state = false;

      bool last_state = last_screen.scr[screen_coords] == 1;
      bool difference = last_state ^ target_state;
      if (difference) {
        flipdot_set_pixel_xy(x, y, target_state);
        slog(target_state ? ">" : "<");
      } else {
        slog(" ");
      }

      last_screen.scr[screen_coords] = target_state ? 1 : 0;
    }
    slog("\n");
  }
}