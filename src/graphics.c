#include "graphics.h"

bool screen_buffer[2048];
int16_t pixel_size;

void init_window(uint16_t width, uint16_t height)
{
    pixel_size = fmin(width / SCREEN_WIDTH, height / SCREEN_HEIGHT);
    InitWindow(pixel_size * SCREEN_WIDTH, pixel_size * SCREEN_HEIGHT, "CHIP8");

    clear_screen();
}

void update_screen(uint8_t frame_buffer[64 * 32])
{
    BeginDrawing();

    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            if (screen_buffer[y * SCREEN_WIDTH + x])
            {
                DrawRectangle(x * pixel_size, y * pixel_size, pixel_size, pixel_size, GREEN);
            }
        }
    }

    EndDrawing();
}

void clear_screen()
{
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            screen_buffer[y * SCREEN_WIDTH + x] = false;
        }
    }
    ClearBackground(BLACK);
}
