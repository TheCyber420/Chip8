#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

void init_window(uint16_t width, uint16_t height);
void update_screen(uint8_t frame_buffer[256]);
void clear_screen(void);