#pragma once

#include <cstddef>


#ifndef SCREEN_H
#define SCREEN_H 9
#endif

#ifndef SCREEN_W
#define SCREEN_W 28
#endif

#define SCREEN_SZ (SCREEN_W * SCREEN_H)


typedef struct {
  size_t x;
  uint8_t scr[SCREEN_SZ];
  uint8_t scr_mask[SCREEN_SZ];
} screen_cursor_t;

// screen config
inline screen_cursor_t screen_cursor;

inline const int n_screen_chars = 5;
inline char screen_chars[n_screen_chars+1];


void init_cursor(screen_cursor_t *cur);

// Print screen to serial
void print_screen(screen_cursor_t *cur);

#define ERR_NO_CHAR_FONT -1
#define ERR_OVERFLOW -2
int put_char(screen_cursor_t *cur, char c);

int put_string(screen_cursor_t *cur, const char *str);

int put_string_animation(screen_cursor_t *cur, const char *str, int n_segments, uint32_t delay_ms_per_segment, bool reverse = false);

void clear_screen(screen_cursor_t *cur);

