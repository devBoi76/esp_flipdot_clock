#include "minifont.h"
#include "clock_display.hpp"
#include "util.h"

#include <memory.h>
#include <Arduino.h>

bool check_error(int screen_error, const char *tag) {
  if (screen_error == 0) return false;

  Serial.print("[Screen Error] : ");
  Serial.print(tag);

  switch(screen_error) {
    case ERR_NO_CHAR_FONT: Serial.println("ERR_NO_CHAR_FONT"); break;
    case ERR_OVERFLOW: Serial.println("ERR_OVERFLOW"); break;
    default: Serial.print("Unknown: "); Serial.println(screen_error); break;
  }

  return true;
}

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

  if (cur->x + fnt->advance > SCREEN_W) return ERR_OVERFLOW;

  for (size_t y = 0; y < CHAR_H; y += 1) {
    for (size_t x = 0; x < fnt->advance; x += 1) {
      cur->scr[y*SCREEN_W + cur->x + x] = fnt->data[y*fnt->advance + x];
    }
  }
  cur->x += (fnt->advance + fnt->gap);

  return 0;
}

int put_string(screen_cursor_t *cur, const char *str) {
  while (*str != 0) {
    int err = put_char(cur, *str);
    if (check_error(err, "put_string()")) return err;

    str += 1;
  }
  return 0;
}

int put_string_animation(screen_cursor_t *cur, const char *str, uint32_t delay_ms_per_segment, bool reverse) {
  clear_screen(cur);

  // nie musimy później sprawdzać overflow bo to nam już sprawdzi
  int ret = put_string(cur, str);
  if (check_error(ret, "put_string_animation()")) return ret;

  set_mask(cur, 0^reverse);
  size_t mask_x = 0;
  while (*str != 0) {
    char ch = *str;

    const font_char_t *fnt = get_char_font(ch);
    size_t char_w = fnt->advance + fnt->gap;

    for (size_t x = 0; x < char_w; x += 1) {
      for (size_t y = 0; y < SCREEN_H; y += 1) {
        cur->scr_mask[y * SCREEN_W + x + mask_x] = 1^reverse;
      }
    }


    print_screen(cur);
    mask_x += char_w;
    str++;
    delay(delay_ms_per_segment);
  }

  set_mask(cur, 1^reverse);

  return 0;
}


void flipdot_write_screen(screen_cursor_t *cur);

void print_screen(screen_cursor_t *cur) {
  flipdot_write_screen(cur);
  for (int y = 0; y < SCREEN_H; y += 1) {
    for (int x = 0; x < SCREEN_W; x += 1) {
      if (cur->scr[y*SCREEN_W+x] & cur->scr_mask[y*SCREEN_W+x] == 1)
        slog("#");
      else if (cur->scr[y*SCREEN_W+x] == 1 && cur->scr_mask[y*SCREEN_W+x] == 0)
        slog(".");
      else
        slog(" ");
    }
    slog("\n");
  }
}
