#include "graphics.h"

bool screen_buffer[2048];
int16_t pixel_size;

void init_window(uint16_t width, uint16_t height)
{
    pixel_size = fmin(width / SCREEN_WIDTH, height / SCREEN_HEIGHT);
    InitWindow(pixel_size * SCREEN_WIDTH, pixel_size * SCREEN_HEIGHT, "CHIP8");

    clear_screen();
}

void update_screen(uint8_t frame_buffer[256])
{
    BeginDrawing();
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
        for (int x = 0; x < SCREEN_WIDTH / 8; ++x)
        {
            for (int b = 0; b < 8; ++b)
            {
                // checks all 8 pixels in the frame_buffer bytes
                if ((frame_buffer[y * 8 + x] >> (7 - b) & 1) == 1)
                {
                    DrawRectangle((x * 8 + b) * pixel_size, y * pixel_size, pixel_size, pixel_size, GREEN);
                }
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
