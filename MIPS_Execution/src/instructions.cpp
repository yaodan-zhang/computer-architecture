#include <string>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "initialization.cpp"
using namespace std;

int ToDecimal(string bin){
    int base = 1;
    int num = 0;
    for(int i=0;i<bin.length();i++){
        num+=stoi(bin.substr(bin.length()-i-1,1))*base;
        base*=2;}
    return num;}

int twosComplement(string bin){
    string result="";
    int num=0;
    if (bin.at(0)=='1'){
        for(int i=0;i<bin.length();i++){
            if(bin.at(i)=='1'){result=result+"0";}
            else{result=result+"1";}}
        num=ToDecimal(result)+1;
        return -num;
    }else{return ToDecimal(bin);}}

void add(int rd,int rs,int rt) {reg[rd]=reg[rs]+reg[rt];}
void And(int rd,int rs,int rt){reg[rd]=reg[rs]&reg[rt];}
void DIV(int rs,int rt){lo=reg[rs]/reg[rt];hi=reg[rs]%reg[rt];}
void jalr(int rd,int rs){reg[rd]=pc+4; pc=reg[rs]-4;}//
void jr(int rs){pc=reg[rs]-4;}
void mfhi(int rd){reg[rd]=hi;}
void mflo(int rd){reg[rd]=lo;}
void mthi(int rs){hi=reg[rs];}
void mtlo(int rs){lo=reg[rs];}
void mult(int rs,int rt){hi=(reg[rs]*reg[rt])&0xffffffff00000000;lo=(reg[rs]*reg[rt])&0x00000000ffffffff;}
void nor(int rd,int rs,int rt){reg[rd]=~(reg[rs]|reg[rt]);}
void OR(int rd,int rs,int rt){reg[rd]=reg[rs]|reg[rt];}
void sll(int rd,int rt,int sa){reg[rd]=reg[rt]<<sa;}
void sllv(int rd,int rt,int rs){reg[rd]=reg[rt]<<reg[rs];}
void slt(int rd,int rs,int rt){if(reg[rs]<reg[rt]){reg[rd]=1;} else{reg[rd]=0;}}
void sra(int rd,int rt,int sa){reg[rd]=reg[rt]>>sa;} 
void srav(int rd,int rt,int rs){reg[rd]=reg[rt]>>reg[rs];}
void srl(int rd,int rt,int sa){reg[rd]=(reg[rt]>>sa)&~((0x1<<31)>>(sa-1));}
void srlv(int rd,int rt,int rs){reg[rd]=(reg[rt]>>reg[rs])&~((0x1<<31)>>(reg[rs]-1));}
void sub(int rd,int rs,int rt){reg[rd]=reg[rs]-reg[rt];}
void syscall(std::ifstream& testin,std::ofstream& testout,int& exit){
    string str;
    if(reg[2]==1){
        int n=reg[4];
        testout<<n;}
    else if(reg[2]==4){
        int i=0;
        while(true){
            char ch = mem[reg[4]-shift+i];
            if(ch=='\0'){break;}
            testout<<ch;
            ++i;
        }}
    else if(reg[2]==5){getline(testin,str);reg[2]=stoi(str);}
    else if(reg[2]==8){
        getline(testin,str);
        reg[5]=str.length();
        for(int i=0;i<reg[5];i++){
            mem[reg[4]-shift+i]=str[i];
        }
    }
    else if(reg[2]==9){reg[2]=reg[28]+reg[4]-1;}
    else if(reg[2]==10){exit=1;}
    else if(reg[2]==11){char ch=reg[4];testout<<ch;}
    else if(reg[2]==12){getline(testin,str);reg[2]=str[0];}
    else if(reg[2]==13){open((char*)reg[4],reg[5],reg[6]);}
    else if(reg[2]==14){read(reg[4],(char*)reg[5],reg[6]);}
    else if(reg[2]==15){write(reg[4],(char*)reg[5],reg[6]);}
    else if(reg[2]==16){close(reg[4]);}
    else if(reg[2]==17){exit=1;}
}
void XOR(int rd,int rs,int rt){reg[rd]=reg[rs]^reg[rt];}
void addi(int rt,int rs,int im){reg[rt]=reg[rs]+im;}
void andi(int rt,int rs,int im){reg[rt]=reg[rs]&ToDecimal(bitset<16>(im).to_string());}
void beq(int rs,int rt,int im){if(reg[rs]==reg[rt]){pc=pc+(im*4);}}
void bgez(int rs,int im){if(reg[rs]>=0){pc=pc+(im*4);}}
void bltz(int rs,int im){if(reg[rs]<0){pc=pc+(im*4);}}
void bgtz(int rs,int im){if(reg[rs]>0){pc=pc+(im*4);}}
void blez(int rs,int im){if(reg[rs]<=0){pc=pc+(im*4);}}
void bne(int rs,int rt,int im){if(reg[rs]!=reg[rt]){pc=pc+(im*4);}}
void lb(int rt,int im,int rs,vector<string>&text){
    if(reg[rs]+im-shift>=0){reg[rt]=mem[reg[rs]+im-shift];}
    else{cout<<"fault"<<endl;}}
void lbu(int rt,int im,int rs){
    if(reg[rs]+im-shift>=0){reg[rt]=mem[reg[rs]+im-shift]&0x000000ff;}
    else{cout<<"fault"<<endl;}}//
void lh(int rt,int im,int rs){
    if(reg[rs]+im-shift>=0){reg[rt]=mem[reg[rs]+im-shift]+(mem[reg[rs]+im-shift+1]<<8);}
    else{cout<<"fault"<<endl;}}//
void lhu(int rt,int im,int rs){
    if(reg[rs]+im-shift>=0){reg[rt]=(mem[reg[rs]+im-shift]+(mem[reg[rs]+im-shift+1]<<8))&0x0000ffff;}
    else{cout<<"fault"<<endl;}}//
void lui(int rt,int im){reg[rt]=(im<<16)&0xffff0000;}
void lw(int rt,int im,int rs){
    if(reg[rs]+im-shift>=0){reg[rt]=mem[reg[rs]+im-shift]+(mem[reg[rs]+im-shift+1]<<8)+(mem[reg[rs]+im-shift+2]<<16)+(mem[reg[rs]+im-shift+3]<<24);}
    else{cout<<"fault"<<endl;}}//
void ori(int rt,int rs,int im){reg[rt]=reg[rs]|ToDecimal(bitset<16>(im).to_string());}
void sb(int rt,int im,int rs){mem[reg[rs]+im-shift]=reg[rt];}
void slti(int rt,int rs,int im){if(reg[rs]<im){reg[rt]=1;}else{reg[rt]=0;}}
void sh(int rt,int im,int rs){mem[reg[rs]+im-shift]=reg[rt]&0x000000ff;mem[reg[rs]+im-shift+1]=reg[rt]&0x0000ff00;}
void sw(int rt,int im,int rs){mem[reg[rs]+im-shift]=reg[rt]&0x000000ff;
mem[reg[rs]+im-shift+1]=reg[rt]&0x0000ff00;
mem[reg[rs]+im-shift+2]=reg[rt]&0x00ff0000;
mem[reg[rs]+im-shift+3]=reg[rt]&0xff000000;
}//
void xori(int rt,int rs,int im){reg[rt]=reg[rs]^ToDecimal(bitset<16>(im).to_string());}
void lwl(int rt,int im,int rs){
    int var=(reg[rs]+im-shift)%4;
    switch(var){
    case 0:reg[rt]=mem[reg[rs]+im-shift]&0x000000ff+mem[reg[rs]+im-shift+1]&0x0000ff00+mem[reg[rs]+im-shift+2]&0x00ff0000+mem[reg[rs]+im-shift+3]&0xff000000;
    case 1:reg[rt]=mem[reg[rs]+im-shift]&0x000000ff+mem[reg[rs]+im-shift+1]&0x0000ff00+mem[reg[rs]+im-shift+2]&0x00ff0000+reg[rt]&0xff000000;
    case 2:reg[rt]=mem[reg[rs]+im-shift]&0x000000ff+mem[reg[rs]+im-shift+1]&0x0000ff00+reg[rt]&0xffff0000;
    case 3:reg[rt]=mem[reg[rs]+im-shift]&0x000000ff+reg[rt]&0xffffff00;
    default: break;
    }}
void lwr(int rt,int im,int rs){
    int var=(reg[rs]+im-shift)%4;
    switch(var){
    case 0:reg[rt]=reg[rt]&0x00ffffff+mem[reg[rs]+im-shift]&0xff000000;
    case 1:reg[rt]=reg[rt]&0x0000ffff+mem[reg[rs]+im-shift]&0xff000000+mem[reg[rs]+im-shift-1]&0x00ff0000;
    case 2:reg[rt]=reg[rt]&0x000000ff+mem[reg[rs]+im-shift]&0xff000000+mem[reg[rs]+im-shift-1]&0x00ff0000+mem[reg[rs]+im-shift-2]&0x0000ff00;
    case 3:reg[rt]=mem[reg[rs]+im-shift]&0xff000000+mem[reg[rs]+im-shift-1]&0x00ff0000+mem[reg[rs]+im-shift-2]&0x0000ff00+mem[reg[rs]+im-shift-3]&0x000000ff;
    default: break;
    }
}
void swl(int rt,int im,int rs){
    int var=(reg[rs]+im-shift)%4;
    switch(var){
    case 0:
    mem[reg[rs]+im-shift]=reg[rt]&0x000000ff;
    mem[reg[rs]+im-shift+1]=reg[rt]&0x0000ff00;
    mem[reg[rs]+im-shift+2]=reg[rt]&0x00ff0000;
    mem[reg[rs]+im-shift+3]=reg[rt]&0xff000000;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+3+1)reg[28]=reg[rs]+im-(shift-0x500000)+3+1;
    case 1:
    mem[reg[rs]+im-shift]=reg[rt]&0x000000ff;
    mem[reg[rs]+im-shift+1]=reg[rt]&0x0000ff00;
    mem[reg[rs]+im-shift+2]=reg[rt]&0x00ff0000;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+2+1)reg[28]=reg[rs]+im-(shift-0x500000)+2+1;
    case 2:
    mem[reg[rs]+im-shift]=reg[rt]&0x000000ff;
    mem[reg[rs]+im-shift+1]=reg[rt]&0x0000ff00;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+1+1)reg[28]=reg[rs]+im-(shift-0x500000)+1+1;
    case 3:
    mem[reg[rs]+im-shift]=reg[rt]&0x000000ff;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+1)reg[28]=reg[rs]+im-(shift-0x500000)+1;
    default: break;
    }
}
void swr(int rt,int im,int rs){
    int var=(reg[rs]+im-shift)%4;
    switch(var){
    case 0:
    mem[reg[rs]+im-shift]=reg[rt]&0xff000000;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+1)reg[28]=reg[rs]+im-(shift-0x500000)+1;
    case 1:
    mem[reg[rs]+im-shift]=reg[rt]&0xff000000;
    mem[reg[rs]+im-shift-1]=reg[rt]&0x00ff0000;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+1)reg[28]=reg[rs]+im-(shift-0x500000)+1;
    case 2:
    mem[reg[rs]+im-shift]=reg[rt]&0xff000000;
    mem[reg[rs]+im-shift-1]=reg[rt]&0x00ff0000;
    mem[reg[rs]+im-shift-2]=reg[rt]&0x0000ff00;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+1)reg[28]=reg[rs]+im-(shift-0x500000)+1;
    case 3:
    mem[reg[rs]+im-shift]=reg[rt]&0xff000000;
    mem[reg[rs]+im-shift-1]=reg[rt]&0x00ff0000;
    mem[reg[rs]+im-shift-2]=reg[rt]&0x0000ff00;
    mem[reg[rs]+im-shift-3]=reg[rt]&0x000000ff;
    if(reg[28]<reg[rs]+im-(shift-0x500000)+1)reg[28]=reg[rs]+im-(shift-0x500000)+1;
    default: break;
    }
}
void j(int tar){pc=pc&0xf0000000+(tar*4)-4;}
void jal(int tar){reg[31]=pc+4;pc=pc&0xf0000000+(tar*2)-4;}
