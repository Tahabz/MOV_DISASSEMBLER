#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define MOV 0x88
#define MOV_MASK 0xFC
#define MASK 0xFF
#define DMASK 0x2
#define DMOD 0x2
#define DREG 0x2
#define WMASK 0x1
#define MMASK 0xC0
#define RMOD 0xC0
#define RMASK 0x38
#define REMASK 0x7

char *table[][8] = {
    {"AL", "AX"},
    {"CL", "CX"},
    {"DL", "DX"},
    {"BL", "BX"},
    {"AH", "SP"},
    {"CH", "BP"},
    {"DH", "SI"},
    {"BH", "DI"}};

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("please enter the file name!");
        return EXIT_FAILURE;
    }

    unsigned char buffer[2];
    char *file_name = argv[1];
    FILE *source;
    FILE *output;

    if (!(source = fopen(file_name, "rb")))
    {
        printf("source does not exist or can't be read");
        return EXIT_FAILURE;
    }

    if (!(output = fopen("output.asm", "w")))
    {
        printf("output can't be created");
        return EXIT_FAILURE;
    }
    fprintf(output, "BITS 16\n\n");
    while (!feof(source))
    {
        buffer[0] = fgetc(source);
        buffer[1] = fgetc(source);

        if ((buffer[0] & MOV_MASK) != MOV)
            continue;
        if ((buffer[1] & MMASK) != RMOD)
            continue;

        unsigned char reg = (buffer[1] & RMASK) >> 3;
        unsigned char reg2 = buffer[1] & REMASK;
        fprintf(output, "MOV ");
        if ((buffer[0] & WMASK) == 0)
        {
            if ((buffer[0] & DMASK) == DMOD)
            {
                fprintf(output, "%s, ", table[reg][0]);
                fprintf(output, "%s\n", table[reg2][0]);
            }
            else
            {
                fprintf(output, "%s, ", table[reg2][0]);
                fprintf(output, "%s\n", table[reg][0]);
            }
        }
        else
        {
            if ((buffer[0] & DMASK) == DMOD)
            {
                fprintf(output, "%s, ", table[reg][1]);
                fprintf(output, "%s\n", table[reg2][1]);
            }
            else
            {
                fprintf(output, "%s, ", table[reg2][1]);
                fprintf(output, "%s\n", table[reg][1]);
            }
        }
    }

    fclose(source);
    fclose(output);

    return 0;
}