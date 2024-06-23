#include "chip8.h"
Chip *chip;
void chip_startup(void)
{
    chip = (Chip *)malloc(sizeof(Chip));
    init_window(1000, 1000);
    // chip->registers[0] = 0;
    // chip->registers[1] = 0;
    // chip->r_I = 1000;
    // chip->memory[1000] = 255;
    // chip->memory[1001] = 255;
    // chip->memory[1002] = 153;
    // chip->memory[1003] = 153;
    // chip->memory[1004] = 231;
    // chip->memory[1005] = 195;
    // chip->memory[1006] = 195;
    // chip->memory[1007] = 219;
    // process_opcode(0xd018);
    process_opcode(0xA29A);
    printf("%d\n", chip->r_I);
    WaitTime(2);
    chip_free();
}
void process_opcode(uint16_t opcode)
{
    switch ((opcode & 0xF000))
    {
    case 0X0000:
        if ((opcode & 0xFF) == 0x00E0)
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
        if (chip->registers[(opcode >> 8) & 0xF] == (opcode & 0xFF))
        {
            chip->pc += 2;
        }
        break;
    case 0x4000:
        // SNE Vx, byte
        if (chip->registers[(opcode >> 8) & 0xF] != (opcode & 0xFF))
        {
            chip->pc += 2;
        }
        break;
    case 0x5000:
        // SE Vx, Vy
        if (chip->registers[(opcode >> 8) & 0xF] == chip->registers[(opcode >> 4) & 0xF])
        {
            chip->pc += 2;
        }
        break;
    case 0x6000:
        // LD Vx, byte
        chip->registers[(opcode >> 8) & 0xF] = (opcode & 0xFF);
        break;
    case 0x7000:
        // ADD Vx, byte
        chip->registers[(opcode >> 8) & 0xF] += (opcode & 0xFF);
        break;
    case 0x8000:
    {
        byte Vx = (opcode >> 8) & 0xF;
        byte Vy = (opcode >> 4) & 0xF;
        switch (opcode & 0xF)
        {
        case 0:
            // LD Vx, Vy
            chip->registers[Vx] = chip->registers[Vy];
            break;
        case 1:
            // OR Vx, Vy
            chip->registers[Vx] |= chip->registers[Vy];
            break;
        case 2:
            // AND Vx, Vy
            chip->registers[Vx] &= chip->registers[Vy];
            break;
        case 3:
            // XOR Vx, Vy
            chip->registers[Vx] ^= chip->registers[Vy];
            break;
        case 4:
            // ADD Vx, Vy
            if (chip->registers[Vx] + chip->registers[Vy] > 255)
            {
                chip->registers[15] = 1;
            }
            else
            {
                chip->registers[15] = 0;
            }
            chip->registers[Vx] += chip->registers[Vy];
            break;
        case 5:
            // SUB Vx, Vy
            if (chip->registers[Vx] > chip->registers[Vy])
            {
                chip->registers[15] = 1;
            }
            else
            {
                chip->registers[15] = 0;
            }
            chip->registers[Vx] -= chip->registers[Vy];
            break;
        case 6:
            // SHR Vx {, Vy}
            chip->registers[15] = (chip->registers[Vx] & 1); // check if lsb is lost
            chip->registers[Vx] = chip->registers[Vx] >> 1;
            break;
        case 7:
            // SUBN Vx, Vy
            if (chip->registers[Vy] > chip->registers[Vx])
            {
                chip->registers[15] = 1;
            }
            else
            {
                chip->registers[15] = 0;
            }
            chip->registers[Vx] = chip->registers[Vy] - chip->registers[Vx];
            break;
        case 0xE:
            // SHR Vx {, Vy}
            chip->registers[15] = (chip->registers[Vx] >> 7); // check value will overflow
            chip->registers[Vx] = chip->registers[Vx] << 1;
            break;
        default:
            printf("invalid\n");
            break;
        }
    }
    case 0x9000:
        // SNE: Vx, Vy
        if (((opcode >> 4) & 0xF) != ((opcode >> 8) & 0xF))
        {
            chip->pc += 2;
        }
        break;
    case 0xa000:
        // LD I, addr
        chip->r_I = (opcode & 0xFFF);
        break;
    case 0xb000:
        // JP V0, addr
        chip->pc = (opcode & 0xFFF) + chip->registers[0];
        break;
    case 0xc000:
        // RND Vx, byte
        chip->registers[(opcode >> 8) & 0xF] = (opcode & 0xFF) & GetRandomValue(0, 256);
        break;
    case 0xd000:
    {
        // DRW Vx, Vy, nibble
        byte Vx = chip->registers[(opcode >> 8) & 0xF] % 8;
        byte Vy = chip->registers[(opcode >> 4) & 0xF] % 32;
        byte n = opcode & 0xF;
        for (int r = 0; r < n; ++r)
        {
            byte sprite_data = chip->memory[chip->r_I + r];
            uint16_t idx = (r + Vy) * 8 + Vx;
            if ((chip->frame_buffer[idx] & sprite_data) != 0)
            {
                chip->registers[15] = 1;
            }
            else
            {
                chip->registers[15] = 0;
            }
            chip->frame_buffer[idx] ^= sprite_data;
        }
        update_screen(chip->frame_buffer);
        break;
    }
    default:
        printf("not a valid opcode\n");
        break;
    }
}

void chip_free(void)
{
    free(chip);
}