#include "chip8.h"

Chip *chip;
void chip_startup(void)
{
    chip = (Chip *)malloc(sizeof(Chip));
    process_opcode(0x8120);
    chip_free();
}
void process_opcode(uint16_t opcode)
{
    switch ((opcode & 0xF000))
    {
    case 0X0000:
        if ((opcode & 0x00FF) == 0x00E0)
        {
            // CLS: clear the screen
            // clear_screen();
            printf("cleared screen\n");
        }
        else
        {
            // RET: return from subroutine
            if (chip->sp > 0)
            {
                chip->pc = chip->stack[chip->sp--];
            }
            else
            {
                printf("stack pointer already at 0\n");
                quick_exit(-1);
            }
        }

        break;
    case 0x1000:
        // JMP addr
        chip->pc = (opcode & 0xFFF);
        break;
    case 0x2000:
        // CALL addr
        chip->stack[++chip->sp] = chip->pc;
        chip->pc = (opcode & 0xFFF);
        break;
    case 0x3000:
        // SE Vx, byte
        if (chip->registers[(opcode >> 8) & 0x0F] == (opcode & 0xFF))
        {
            chip->pc += 2;
        }
        break;
    case 0x4000:
        // SNE Vx, byte
        if (chip->registers[(opcode >> 8) & 0x0F] != (opcode & 0xFF))
        {
            chip->pc += 2;
        }
        break;
    case 0x5000:
        // SE Vx, Vy
        if (chip->registers[(opcode >> 8) & 0x0F] == chip->registers[(opcode >> 4) & 0x00F])
        {
            chip->pc += 2;
        }
        break;
    case 0x6000:
        // LD Vx, byte
        chip->registers[(opcode >> 8) & 0x0F] = (opcode & 0xFF);
        break;
    case 0x7000:
        // ADD Vx, byte
        chip->registers[(opcode >> 8) & 0x0F] += (opcode & 0xFF);
        break;
    case 0x8000:
        switch (opcode & 0xF)
        {
        case 0:
            // LD Vx, Vy
            chip->registers[(opcode >> 8) & 0x0F] = chip->registers[(opcode >> 4) & 0x00F];
            break;
        default:
            printf("invalid\n");
            break;
        }
        break;

    default:
        printf("not a valid opcode\n");
        break;
    }
}

void chip_free(void)
{
    free(chip);
}