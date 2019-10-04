#ifndef CPU_INSTRUCTIONS_H
#define CPU_INSTRUCTIONS_H
#include "mainwindow.h"


class cpu_instructions
{
public:
    std::vector<unsigned char> * rom_buff;
    unsigned int *rom_count;
    cpu_instructions();
    void set_buff_count(std::vector<unsigned char> * rom_b,unsigned int *rom_c);
    //TO instructions
    /*
    A = 0x97;	 3197H	Immediate number 97H to register A
    B = 0x52;	 3252H	Immediate number 52H to register B
    A0 = 0x00;	 3300H	Immediate number 00H to register A0
    A1 = 0xff;	 34FFH	Immediate number 34H to register A1
    A2 = 0xe9;	 35E9H	Immediate number E9H to register A2
    RAM = 0x3C;	 373CH	Immediate number 3CH to RAM/IO;
    */
    void I_A_set(unsigned char x);
    void I_B_set(unsigned char x);
    void I_A0_set(unsigned char x);
    void I_A1_set(unsigned char x);
    void I_A2_set(unsigned char x);
    void I_RAM_set(unsigned char x);
    /*
    A = A;	 11FFH	register A to register A
    B = A;	 12FFH	register A to register B
    A0 = A;	 13FFH	register A to register A0
    A1 = A;	 14FFH	register A to register A1
    A2 = A;	 15FFH	register A to register A2
    RAM = A;	 17FFH	register A to register RAM/IO
    */
    void I_A_TO_A();
    void I_A_TO_B();
    void I_A_TO_A0();
    void I_A_TO_A1();
    void I_A_TO_A2();
    void I_A_TO_RAM();
    /*
    A = B;	 11AFH	register B to register A
    B = B;	 12AFH	register B to register B
    A0 = B;	 13AFH	register B to register A0
    A1 = B;	 14AFH	register B to register A1
    A2 = B;	 15AFH	register B to register A2
    RAM = B;	 17AFH	register B to register RAM/IO
    */
    void I_B_TO_A();
    void I_B_TO_B();
    void I_B_TO_A0();
    void I_B_TO_A1();
    void I_B_TO_A2();
    void I_B_TO_RAM();

    /*
    A = RAM;	 21FFH	 RAM/IO to register A
    B = RAM;	 22FFH	 RAM/IO to register B
    A0 = RAM;	 23FFH	 RAM/IO to register A0
    A1 = RAM;	 24FFH	 RAM/IO to register A1
    A2 = RAM;	 25FFH	 RAM/IO to register A2
    RAM = RAM;	 27FFH	 RAM/IO to RAM/IO
    */
    void I_RAM_TO_A();
    void I_RAM_TO_B();
    void I_RAM_TO_A0();
    void I_RAM_TO_A1();
    void I_RAM_TO_A2();
    void I_RAM_TO_RAM();

    /*
    JNAH_A2A1A0;	 09FFH	if register A's most significant bit is 0,then jump to A2A1A0 address
    JNBH_A2A1A0;	 0AFFH	if register B's most significant bit is 0,then jump to A2A1A0 address
    JANB_A2A1A0;	 0B9FH	if register A is not equal to register B,then jump to A2A1A0 address
    JAAB_A2A1A0;	 0C94H	if register A + register B has carry bit, then jump to A2A1A0 address
    JAAA_A2A1A0;	 0CC4H	if register A + register A has carry bit, then jump to A2A1A0 address
    JAAI_A2A1A0;	 0C00H	if register A + 1 has carry bit, then jump to A2A1A0 address
    JAABI_A2A1A0;	 0C90H	if register A + register B + 1 has carry bit, then jump to A2A1A0 address
    JAAAI_A2A1A0;	 0CC0H	if register A + register A + 1 has carry bit, then jump to A2A1A0 address
    JNSAB_A2A1A0;	 0C60H	if register A - register B does not have borrow bit, then jump to A2A1A0 address
    JNSAI_A2A1A0;	 0CF4H	if register A - 1 does not have borrow bit, then jump to A2A1A0 address
    JNSABI_A2A1A0;	 0C64H	if register A - register B - 1 does not have borrow bit, then jump to A2A1A0 address
    JMP_A2A1A0;	 0DFFH	unconditionally jump A2A1A0 address

    NOP;	 FFFFH	Empty operation
    NOP;	 0000H	Empty operation
    */
    void I_NOP();
    void I_JNAH_A2A1A0();
    void I_JNBH_A2A1A0();
    void I_JANB_A2A1A0();
    void I_JAAB_A2A1A0();
    void I_JAAA_A2A1A0();
    void I_JAAI_A2A1A0();
    void I_JAABI_A2A1A0();
    void I_JAAAI_A2A1A0();
    void I_JNSAB_A2A1A0();
    void I_JNSAI_A2A1A0();
    void I_JNSABI_A2A1A0();
    void I_JMP_A2A1A0();


};

#endif // CPU_INSTRUCTIONS_H
