#include "cpu_instructions.h"


cpu_instructions::cpu_instructions()
{
    this->rom_buff=NULL;
    this->rom_count=NULL;
}
void cpu_instructions::set_buff_count(std::vector<unsigned char> * rom_b,unsigned int *rom_c)
{
    this->rom_buff = rom_b;
    this->rom_count = rom_c;

}
//TO instructions
/*
A = 0x97;	 3197H	Immediate number 97H to register A
B = 0x52;	 3252H	Immediate number 52H to register B
A0 = 0x00;	 3300H	Immediate number 00H to register A0
A1 = 0xff;	 34FFH	Immediate number 34H to register A1
A2 = 0xe9;	 35E9H	Immediate number E9H to register A2
RAM = 0x3C;	 373CH	Immediate number 3CH to RAM/IO;
*/
void cpu_instructions::I_A_set(unsigned char x)
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x31);
        this->rom_buff->push_back(x);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_B_set(unsigned char x)
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x32);
        this->rom_buff->push_back(x);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_A0_set(unsigned char x)
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x33);
        this->rom_buff->push_back(x);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_A1_set(unsigned char x)
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x34);
        this->rom_buff->push_back(x);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_A2_set(unsigned char x)
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x35);
        this->rom_buff->push_back(x);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_RAM_set(unsigned char x)
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x37);
        this->rom_buff->push_back(x);
        *(this->rom_count)+=2;
    }
}
/*
A = A;	 11FFH	register A to register A
B = A;	 12FFH	register A to register B
A0 = A;	 13FFH	register A to register A0
A1 = A;	 14FFH	register A to register A1
A2 = A;	 15FFH	register A to register A2
RAM = A;	 17FFH	register A to register RAM/IO
*/
void cpu_instructions::I_A_TO_A()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x11);
        this->rom_buff->push_back(0xff);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_A_TO_B()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x12);
        this->rom_buff->push_back(0xff);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_A_TO_A0()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x13);
        this->rom_buff->push_back(0xff);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_A_TO_A1()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x14);
        this->rom_buff->push_back(0xff);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_A_TO_A2()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x15);
        this->rom_buff->push_back(0xff);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_A_TO_RAM()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x17);
        this->rom_buff->push_back(0xff);
        *(this->rom_count)+=2;
    }
}
/*
A = B;	 11AFH	register B to register A
B = B;	 12AFH	register B to register B
A0 = B;	 13AFH	register B to register A0
A1 = B;	 14AFH	register B to register A1
A2 = B;	 15AFH	register B to register A2
RAM = B;	 17AFH	register B to register RAM/IO
*/
void cpu_instructions::I_B_TO_A()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x11);
        this->rom_buff->push_back(0xAF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_B_TO_B()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x12);
        this->rom_buff->push_back(0xAF);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_B_TO_A0()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x13);
        this->rom_buff->push_back(0xAF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_B_TO_A1()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x14);
        this->rom_buff->push_back(0xAF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_B_TO_A2()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x15);
        this->rom_buff->push_back(0xAF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_B_TO_RAM()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x17);
        this->rom_buff->push_back(0xAF);
        *(this->rom_count)+=2;
    }
}

/*
A = RAM;	 21FFH	 RAM/IO to register A
B = RAM;	 22FFH	 RAM/IO to register B
A0 = RAM;	 23FFH	 RAM/IO to register A0
A1 = RAM;	 24FFH	 RAM/IO to register A1
A2 = RAM;	 25FFH	 RAM/IO to register A2
RAM = RAM;	 27FFH	 RAM/IO to RAM/IO
*/
void cpu_instructions::I_RAM_TO_A()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x21);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_RAM_TO_B()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x22);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_RAM_TO_A0()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x23);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_RAM_TO_A1()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x24);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_RAM_TO_A2()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x25);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}
void cpu_instructions::I_RAM_TO_RAM()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x27);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}







//////////////////////////////////////////////////////////////////////////////////////////////
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

void cpu_instructions::I_NOP()
{
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x00);
        this->rom_buff->push_back(0x00);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JNAH_A2A1A0()
{
    //if register A's most significant bit is 0,then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x09);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JNBH_A2A1A0()
{
    //if register B's most significant bit is 0,then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0A);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}


void cpu_instructions::I_JANB_A2A1A0()
{
    //if register A is not equal to register B,then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0B);
        this->rom_buff->push_back(0x9F);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JAAB_A2A1A0()
{
    //if register A + register B has carry bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0x94);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JAAA_A2A1A0()
{
    //if register A + register A has carry bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0xC4);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JAAI_A2A1A0()
{
    //if register A + 1 has carry bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0x00);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JAABI_A2A1A0()
{
    //if register A + register B + 1 has carry bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0x90);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JAAAI_A2A1A0()
{
    //if register A + register A + 1 has carry bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0xC0);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JNSAB_A2A1A0()
{
    //if register A - register B does not have borrow bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0x60);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JNSAI_A2A1A0()
{
    //if register A - 1 does not have borrow bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0xF4);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JNSABI_A2A1A0()
{
    //if register A - register B - 1 does not have borrow bit, then jump to A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0C);
        this->rom_buff->push_back(0x64);
        *(this->rom_count)+=2;
    }
}

void cpu_instructions::I_JMP_A2A1A0()
{
    //unconditionally jump A2A1A0 address
    if(this->rom_buff)
    {
        this->rom_buff->push_back(0x0D);
        this->rom_buff->push_back(0xFF);
        *(this->rom_count)+=2;
    }
}






//use the command above !!!!
void cpu_instructions::I_JUMP(unsigned int x)
{
    this->I_A1_set((x>>8));
    this->I_A0_set(x);
    this->I_JMP_A2A1A0();
}

//RAM address changes
void cpu_instructions::I_LOADRAM_TO_A(unsigned int x)
{
    //gets data from A and stores to the A2A1A0 address
    this->I_A1_set((unsigned char)(x>>8));
    this->I_A0_set((unsigned char)x);
    this->I_A_TO_RAM();
}

void cpu_instructions::I_LOADA_TO_RAM(unsigned int x)
{
    //gets data from A and stores to the A2A1A0 address
    this->I_A1_set((unsigned char)(x>>8));
    this->I_A0_set((unsigned char)x);
    this->I_RAM_TO_A();
}


//pages stuff , x is actually the address not page number!!!
void cpu_instructions::I_PAGE_JUMP(unsigned int x)
{
    //split the x into 0,1,2 memory of the page before we jump
    this->I_A_set((unsigned char)(x>>16));
    this->I_LOADA_TO_RAM(PAGE_CS);// store x 24-16 to address 2
    this->I_A_set((unsigned char)(x>>8));
    this->I_LOADA_TO_RAM(PAGE_ADD_H);//store x 16-8  to address 1
    this->I_A_set((unsigned char)x);
    this->I_LOADA_TO_RAM(PAGE_ADD_L);//store x 8-0 to address 0
    this->I_JUMP((SIZE_PER_PAGE - SIZE_PAGE_JUMP));//jump to very end of page address
}
//
void cpu_instructions::I_FUN_RETURN_IN_PAGE(unsigned int ret_address)
{
    //load ram data to A
    this->I_A1_set((ret_address>>8));
    this->I_A0_set(ret_address);
    this->I_RAM_TO_A();
    //load ram data to A1
    this->I_A1_set(((ret_address+1)>>8));
    this->I_A0_set((ret_address+1));
    this->I_RAM_TO_A1();
    //load A to A0
    this->I_A_TO_A0();
    //jump
    this->I_JMP_A2A1A0();
}
//
void cpu_instructions::I_FUN_RETURN_BETWEEN_PAGES(unsigned int ret_address)
{
    //split the x into 0,1,2 memory of the page before we jump
    this->I_LOADRAM_TO_A(ret_address);
    this->I_LOADA_TO_RAM(PAGE_ADD_L);

    this->I_LOADRAM_TO_A((ret_address+1));
    this->I_LOADA_TO_RAM(PAGE_ADD_H);

    this->I_LOADRAM_TO_A((ret_address+2));
    this->I_LOADA_TO_RAM(PAGE_CS);

    this->I_JUMP((SIZE_PER_PAGE - SIZE_PAGE_JUMP));
}
