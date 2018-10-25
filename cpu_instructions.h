#ifndef CPU_INSTRUCTIONS_H
#define CPU_INSTRUCTIONS_H
#include<QString>


#define IMMEDIATE_TO_A  0
#define IMMEDIATE_TO_R1  1
#define IMMEDIATE_TO_R2  2
#define IMMEDIATE_TO_R3  3

#define RAM_TO_A    16
#define RAM_TO_R1    17
#define RAM_TO_R2    18
#define RAM_TO_R3    19

#define A_TO_RAM    32
#define A_TO_A  48
#define A_TO_R1  49
#define A_TO_R2  50
#define A_TO_R3  51

#define A_ADD_R1  64
#define A_ADD_1 80
#define A_SUB_R1    96
#define A_SUB_1 112
#define A_AND_R1    128
#define A_OR_R1 144
#define A_NOT  160
#define A_XOR_R1    176


class cpu_instructions
{
public:
    QString rom_buff;
    int rom_count;
    cpu_instructions();
};

#endif // CPU_INSTRUCTIONS_H
