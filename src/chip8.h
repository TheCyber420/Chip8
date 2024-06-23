#include <stddef.h>
#include <stdlib.h>
#include "graphics.h"

typedef uint8_t byte;
typedef struct Architecture
{
    byte memory[4096];          // 512 -> 4095 available
    byte frame_buffer[64 * 32]; // stores pixel data
    byte registers[16];         // 16 8-bit registers from V0 -> VF
    uint16_t stack[16];         // array of length 16 with 16 bit values
    uint16_t r_I;               // special I register
    byte t_sound;               // sound timer register
    byte t_delay;               // delay timer register
    uint16_t pc;                // 16-bit program counter
    byte sp;                    // 8-bit stack pointer

} Chip;

void chip_startup(void);
void chip_free(void);
void process_opcode(uint16_t opcode);