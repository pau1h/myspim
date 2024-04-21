#include "spimcore.h"


void bin(unsigned hex){ //takes a hex number and prints out binary value
    for(int i = 31; i >= 0; i--){
        printf("%u", (hex>>i & 1));
    }
    printf("\n");
}
/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) //A and B are input parameters. Implement operations on A and B according to ALUControl.
{
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) //store the instruction in instruction ptr. returns 1 if halt sequence is required.
{
    if((PC % 4 != 0) || PC < 0x0000 || PC > 0xFFFF){ //halt sequence is reached if PC is out of range of memory, or PC is not word aligned.
        return 1;
    }
    *instruction = MEM(PC); //MEM is defined in spimcore.c as Mem[PC >> 2]
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) //partition instruction into r1, r2, r3, funct, offset, and jsec
{
    *op = instruction >> 26;
    *r1 = ((instruction >> 21) & 0x1F);
    *r2 = ((instruction >> 16) & 0x1F);
    *r3 = ((instruction >> 11) & 0x1F); 
    *funct = instruction & 0x1F;
    *offset = instruction & 0x7FFF;
    *jsec = instruction & 0x1FFFFFF;
}


/*
21080001 -> 0010 0001 0000 1000 0000 0000 0000 0001 -> 001000 01000 01000 0000000000000001
                                                        op      rs  rt      offset (immediate val)

*/
/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    //for memread, write, or regwrite 0 = disabled, 1 = enabled, 2 = dont care
    //for regdst, jump, branch, memtoreg, or alusrc, the value of 0 or 1 indicates the selected path of the multiplexer; 2 = dont care
    //TODO: change to switch statement
    if(op == 0){ //R type instruction
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls-> MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 7;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
    }else if(op == 0x23){ //load word from memory to register
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }else if(op == 0x2B){ //store word from register to mem
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0;
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        controls->RegWrite = 0;
    }else if(op == 0x4){ //beq
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 1; //aluop set to subtract. if r1 - r2 == 0, registers are equal -> branch
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
    }else if(op == 0xA){ //set less than immediate
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 2;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }else if(op == 0xB){ //set less than immediate unsigned 
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 3;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }else if(op == 0x8){ //add immediate 
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }else if(op == 0xF){ //load upper immediate. loads constant in upper 16 bits, fills lower 16 bits with 0s
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 6; //shift left extended_value by 16 bits
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }else if(op == 0x2){ //jump
        controls->RegDst = 2;
        controls->Jump = 1;
        controls->Branch = 2; //DC since jump mux is set to 1
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 2;
        controls->RegWrite = 0;
    }else{
        return 1;
    }
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2) //read registers addressed by r1 and r2 from Reg. write values to data1 and data2.
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
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

