#include <string>
#include <fstream>
#include <bitset>
#include "instructions.cpp"
using namespace std;

void execute(string code,std::ifstream& testin,std::ofstream& testout,int& exit,vector<string>& text){
    if (code.substr(0,6) == "000000") { //R-type
        string funcode = code.substr(26,6);
        int rs=ToDecimal(code.substr(6,5));
        int rt=ToDecimal(code.substr(11,5));
        int rd=ToDecimal(code.substr(16,5));
        int sa=twosComplement(code.substr(21,5));
        if (funcode == "100000"){add(rd,rs,rt);}
        else if (funcode == "100001"){add(rd,rs,rt);}
        else if (funcode == "100100"){And(rd,rs,rt);}
        else if (funcode == "011010"){DIV(rs,rt);}
        else if (funcode == "011011"){DIV(rs,rt);}
        else if (funcode == "001001"){jalr(rd,rs);}
        else if (funcode == "001000"){jr(rs);}
        else if (funcode == "010000"){mfhi(rd);}
        else if (funcode == "010010"){mflo(rd);}
        else if (funcode == "010001"){mthi(rs);}
        else if (funcode == "010011"){mtlo(rs);}
        else if (funcode == "011000"){mult(rs,rt);}
        else if (funcode == "011001"){mult(rs,rt);}
        else if (funcode == "100111"){nor(rd,rs,rt);}
        else if (funcode == "100101"){OR(rd,rs,rt);}
        else if (funcode == "000000"){sll(rd,rt,sa);}
        else if (funcode == "000100"){sllv(rd,rt,rs);}
        else if (funcode == "101010"){slt(rd,rs,rt);}
        else if (funcode == "101011"){slt(rd,rs,rt);}
        else if (funcode == "000011"){sra(rd,rt,sa);}
        else if (funcode == "000111"){srav(rd,rt,rs);}
        else if (funcode == "000010"){srl(rd,rt,sa);}
        else if (funcode == "000110"){srlv(rd,rt,rs);}
        else if (funcode == "100010"){sub(rd,rs,rt);}
        else if (funcode == "100011"){sub(rd,rs,rt);}
        else if (funcode == "001100"){syscall(testin,testout,exit);}
        else if (funcode == "100110"){XOR(rd,rs,rt);}
    }
    else { //I-type and J-type
        string opcode = code.substr(0,6);
        int rs = ToDecimal(code.substr(6,5));
        int rt = ToDecimal(code.substr(11,5));
        int im = twosComplement(code.substr(16,16));
        int tar = ToDecimal(code.substr(6,26));
        if (opcode == "001000"){addi(rt,rs,im);}
        else if (opcode == "001001"){addi(rt,rs,im);}
        else if (opcode == "001100"){andi(rt,rs,im);}
        else if (opcode == "000100"){beq(rs,rt,im);}
        else if (opcode == "000001"){if(rt==1){bgez(rs,im);}else if(rt==0){bltz(rs,im);}}
        else if (opcode == "000111"){if(rt==0){bgtz(rs,im);}}
        else if (opcode == "000110"){if(rt==0){blez(rs,im);}}
        else if (opcode == "000101"){bne(rs,rt,im);}
        else if (opcode == "100000"){lb(rt,im,rs,text);}
        else if (opcode == "100100"){lbu(rt,im,rs);}
        else if (opcode == "100001"){lh(rt,im,rs);}
        else if (opcode == "100101"){lhu(rt,im,rs);}
        else if (opcode == "001111"){lui(rt,im);}
        else if (opcode == "100011"){lw(rt,im,rs);}
        else if (opcode == "001101"){ori(rt,rs,im);}
        else if (opcode == "101000"){sb(rt,im,rs);}
        else if (opcode == "001010"){slti(rt,rs,im);}
        else if (opcode == "001011"){slti(rt,rs,im);}
        else if (opcode == "101001"){sh(rt,im,rs);}
        else if (opcode == "101011"){sw(rt,im,rs);}
        else if (opcode == "001110"){xori(rt,rs,im);}
        else if (opcode == "100010"){lwl(rt,im,rs);}
        else if (opcode == "100110"){lwr(rt,im,rs);}
        else if (opcode == "101010"){swl(rt,im,rs);}
        else if (opcode == "101110"){swr(rt,im,rs);}
        else if (opcode == "000010"){j(tar);}
        else if (opcode == "000011"){jal(tar);}
    }
}