#include "spimcore.h"


void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) //assign 1 to zero if output is 0, else set to 0.
{
    switch(ALUControl){
        case 0: *ALUresult = A + B; //add or DC
        break;
        case 1: *ALUresult = A - B; //sub
        break;
        case 2: *ALUresult = ((int)A < (int)B); //slt(signed)
        break;
        case 3: *ALUresult = (A < B); //slt (unsigned)
        break;
        case 4: *ALUresult = (A&B); //logical AND
        break;
        case 5: *ALUresult = (A|B); //logical OR
        break;
        case 6: *ALUresult = (B<<16); //shift B by 16 bits
        break;
        case 7: *ALUresult = (~A); //NOT
        break;
    }
    *Zero = (*ALUresult == 0 ? 1 : 0); //if aluresult is zero, set Zero = 1
}

int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) //store the instruction in instruction ptr. returns 1 if halt sequence is required.
{
    if((PC % 4 != 0) || PC < 0 || PC > 0xFFFF){ //halt sequence is reached if PC is out of range of memory, or PC is not word aligned.
        return 1;
    }
    *instruction = MEM(PC); //MEM is defined in spimcore.c as Mem[PC >> 2]
    return 0;
}


void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) //partition instruction into r1, r2, r3, funct, offset, and jsec
{
    *op = instruction >> 26;
    *r1 = ((instruction >> 21) & 0x1F);
    *r2 = ((instruction >> 16) & 0x1F);
    *r3 = ((instruction >> 11) & 0x1F); 
    *funct = instruction & 0x3F;
    *offset = instruction & 0xFFFF;
    *jsec = instruction & 0x1FFFFFF;
}


/*
21080001 -> 0010 0001 0000 1000 0000 0000 0000 0001 -> 001000 01000 01000 0000000000000001
                                                        op      rs  rt      offset (immediate val)

*/
int instruction_decode(unsigned op,struct_controls *controls)
{
    //for memread, write, or regwrite 0 = disabled, 1 = enabled, 2 = dont care
    //for regdst, jump, branch, memtoreg, or alusrc, the value of 0 or 1 indicates the selected path of the multiplexer; 2 = dont care
    switch(op){
        case 0: //R Type instruction
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls-> MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        case 0x23: //load word from mem to register
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x2B: //store word from mem to register
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;
        case 0x4: //beq
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 1; //aluop set to subtract. if r1 - r2 == 0, registers are equal -> branch
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        case 0xA: //set less than immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0xB: //set less than immediate unsigned
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x8: //add immedaite
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0xF: //load upper immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6; //shift left extended_value by 16 bits
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x2: //jump
            controls->RegDst = 2;
            controls->Jump = 1;
            controls->Branch = 2; //DC since jump mux is set to 1
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            break;
        default:
            return 1;
            break;
    }
    return 0;
}

void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2) //read registers addressed by r1 and r2 from Reg. write values to data1 and data2.
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

void sign_extend(unsigned offset,unsigned *extended_value) //assign the sign-extended value of offset to extended_value. 16th bit is the sign bit.
{
    if((offset >> 15) & 0x1){ 
        *extended_value = (offset|0xFFFF0000);
    }else{
        *extended_value = offset;
    }
}

int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{ 
    unsigned B = (ALUSrc == 1) ? extended_value : data2; //if alusrc is 1, pull data from extended_val instead of reg 2
    if(ALUOp == 7){ 
        switch(funct){
           case 0x20: ALUOp = 0; //add
           break;
           case 0x22: ALUOp = 1; //sub
           break;
           case 0x24: ALUOp = 4; //and
           break;
           case 0x25: ALUOp = 5; //or
           break;
           case 0x2A: ALUOp = 2; //slt
           break;
           case 0x2B: ALUOp = 3; //sltu
           break;
           default: return 1;
        }
    }
    ALU(data1, B, ALUOp, ALUresult, Zero);
    return 0;
}

int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem) //use the value of memwrite or read to determine of a mem write / read operation is occuring
{
    if(MemRead != 0){ //load word from mem to register
        if((ALUresult % 4 == 0) && ALUresult <= 0xFFFF){  //check if aluresult is word aligned and within mem range
            *memdata = Mem[ALUresult>>2];
        }else{
            return 1;
        }
    }
    if(MemWrite != 0){  //write word to memory
        if((ALUresult % 4 == 0) && ALUresult <= 0xFFFF){
            Mem[ALUresult>>2] = data2;
        }else{
            return 1;
        }
    }
    return 0;
}


void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg) //set r2 or r3 based on regdst to aluresult or memdata.
{
    if(RegWrite == 1){
        Reg[RegDst == 0 ? r2 : r3] = (MemtoReg == 1 ? memdata : ALUresult); 
    }
}

void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC) //update the program counter
{
     *PC += 4; //update program counter
     if(Branch == 1 && Zero == 1){ 
        *PC = *PC + (extended_value<<2);
     }
     if(Jump){
        *PC =  ((*PC & 0xF0000000) | (jsec<<2)); //psudodirect addressing. keep four most sig bits and add jsec to find jump address
     }
}

