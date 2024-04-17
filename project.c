#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) //A and B are input parameters. Implement operations on A and B according to ALUControl.
{
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) //store the instruction in instruction ptr. returns 1 if halt sequence is required.
{
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}


/*
21080001 -> 0010 0001 0000 1000 0000 0000 0000 0001 -> 001000 01000 01000 0000000000000001
                                                        op      rs  rt      offset (immediate val)

*/
/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls) //after getting opcode of instruction
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

