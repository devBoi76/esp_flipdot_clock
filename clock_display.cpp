#include "minifont.h"
#include "clock_display.hpp"

#include <memory.h>
#include <Arduino.h>

void clear_screen(screen_cursor_t *cur) {
  memset(cur->scr, 0, SCREEN_SZ);
  cur->x = 0;
}

void set_mask(screen_cursor_t *cur, uint8_t val) {
  memset(cur->scr_mask, val, SCREEN_SZ);
}


void init_cursor(screen_cursor_t *cur) {
  clear_screen(cur);
  set_mask(cur, 1);
}

int put_char(screen_cursor_t *cur, char c) {
  const font_char_t* fnt = get_char_font(c);
  if (fnt == 0) return ERR_NO_CHAR_FONT;

  if (cur->x + fnt->advance >= SCREEN_W) return ERR_OVERFLOW;

  for (size_t y = 0; y < CHAR_H; y += 1) {
    for (size_t x = 0; x < fnt->advance; x += 1) {
      cur->scr[y*SCREEN_W + cur->x + x] = fnt->data[y*fnt->advance + x];
    }
  }
  cur->x += (fnt->advance + CHAR_GAP);

  return 0;
}

int put_string(screen_cursor_t *cur, const char *str) {
  while (*str != 0) {
    int err = put_char(cur, *str);
    if (err != 0) return err;

    str += 1;
  }
  return 0;
}

// n_segments: 0 for per-character animation
int put_string_animation(screen_cursor_t *cur, const char *str, int n_segments, uint32_t delay_ms_per_segment, bool reverse) {
  cur->x = 0;

  put_string(cur, str);
  set_mask(cur, 0^reverse);
  size_t mask_x = 0;
  while (*str != 0) {
    const font_char_t *ch = get_char_font(*str);
    if (!ch) return -1;
    size_t char_w = ch->advance + CHAR_GAP;

    for (size_t x = 0; x < char_w; x += 1) {
      for (size_t y = 0; y < SCREEN_H; y += 1) {
        cur->scr_mask[y * SCREEN_W + x + mask_x] = 1^reverse;
      }
    }


    print_screen(cur);
    mask_x += char_w;
    str += 1;
    delay(delay_ms_per_segment);
  }

  set_mask(cur, 1^reverse);
}



void print_screen(screen_cursor_t *cur) {
  for (int y = 0; y < SCREEN_H; y += 1) {
    for (int x = 0; x < SCREEN_W; x += 1) {
      if (cur->scr[y*SCREEN_W+x] & cur->scr_mask[y*SCREEN_W+x] == 1)
        Serial.print("#");
      else if (cur->scr[y*SCREEN_W+x] == 1 && cur->scr_mask[y*SCREEN_W+x] == 0)
        Serial.print(".");
      else
        Serial.print(" ");
    }
    Serial.print("\n");
  }
}
