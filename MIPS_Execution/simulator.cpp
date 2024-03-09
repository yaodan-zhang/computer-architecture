#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cstdio>
#include <string>
#include <algorithm>
#include "src/execution.cpp"
using namespace std;

string RemoveComments(string line) {
    int found = line.find("#");
    if (found != -1)return line.substr(0,found);
    return line;}

void writeData(string line){
    line = RemoveComments(line);
    if (line.find(".asciiz")!=-1){//space align for 5 types
        int ind1 = line.find("\"",line.find(".asciiz")+7);
        int ind2 = line.find("\"",ind1+1);
        string asciiz = line.substr(ind1+1,ind2-ind1-1);
        if((reg[28]-0x500000)%4!=0){reg[28]=reg[28]+(4-((reg[28]-0x500000)%4));}//
        for (int i=0;i<asciiz.length();i++){
          if(asciiz.substr(i,1)=="\\"){
            if((i+1)<asciiz.length()){
              if(asciiz[i+1]=='n'){mem[reg[28]-0x500000]='\n';++reg[28];++i;continue;}
              else if(asciiz[i+1]=='0'){mem[reg[28]-0x500000]='\0';++reg[28];++i;continue;}
              else {mem[reg[28]-0x500000]='\\';++reg[28];continue;}
            }
          }
          mem[reg[28]-0x500000]=asciiz[i];++reg[28];}
        mem[reg[28]-0x500000]='\0';++reg[28];
    }
    else if (line.find(".ascii")!=-1){//big endian for case 1,2
        int ind1 = line.find("\"",line.find(".ascii")+6);
        int ind2 = line.find("\"",ind1+1);
        string ascii = line.substr(ind1+1,ind2-ind1-1);
        if((reg[28]-0x500000)%4!=0)reg[28]=reg[28]+(4-((reg[28]-0x500000)%4));
        for (int i=0;i<ascii.length();i++){
          if(ascii.substr(i,1)=="\\"){
            if((i+1)<ascii.length()){
              if(ascii[i+1]=='n'){mem[reg[28]-0x500000]='\n';++reg[28];++i;continue;}
              else if(ascii[i+1]=='0'){mem[reg[28]-0x500000]='\0';++reg[28];++i;continue;}
              else {mem[reg[28]-0x500000]='\\';++reg[28];continue;}
            }
          }
            mem[reg[28]-0x500000]=ascii[i];++reg[28];}
    }
    else if (line.find(".word")!=-1){
        string tmp;tmp.clear();
        for(char v:line){if(v!=' '&&v!='\t')tmp.push_back(v);}
        line = tmp;
        int word = stoi(line.substr(line.find(".word")+5));
        if((reg[28]-0x500000)%4!=0)reg[28]=reg[28]+(4-((reg[28]-0x500000)%4));
        mem[reg[28]-0x500000]=word & 0x000000ff;++reg[28];
        mem[reg[28]-0x500000]=word & 0x0000ff00;++reg[28];
        mem[reg[28]-0x500000]=word & 0x00ff0000;++reg[28];
        mem[reg[28]-0x500000]=word & 0xff000000;++reg[28];
    }
    else if (line.find(".byte")!=-1){
        string tmp;tmp.clear();
        for(char v:line){if(v!=' '&&v!='\t')tmp.push_back(v);}
        line = tmp;
        int ind1=line.find(".byte")+5;
        int ind2=line.find(",",ind1);
        if((reg[28]-0x500000)%4!=0)reg[28]=reg[28]+(4-((reg[28]-0x500000)%4));//
        mem[reg[28]-0x500000]=stoi(line.substr(ind1,ind2-ind1))&0x000000ff;++reg[28];
        for (int i=1;i<4;i++){
            ind1=ind2+1;
            ind2=line.find(",",ind1);
            mem[reg[28]-0x500000]=stoi(line.substr(ind1,ind2-ind1))&0x000000ff;
            ++reg[28];}
    }
    else if (line.find(".half")!=-1){
        string tmp;tmp.clear();
        for(char v:line){if(v!=' '&&v!='\t')tmp.push_back(v);}
        line = tmp;
        int ind1=line.find(".half")+5;
        int ind2=line.find(",",ind1);
        if((reg[28]-0x500000)%4!=0)reg[28]=reg[28]+(4-((reg[28]-0x500000)%4));
        mem[reg[28]-0x500000]=stoi(line.substr(ind1,ind2-ind1))&0x000000ff;++reg[28];
        mem[reg[28]-0x500000]=stoi(line.substr(ind1,ind2-ind1))&0x0000ff00;++reg[28];
        ind1 = ind2+1;
        ind2 = line.find(",",ind1);
        mem[reg[28]-0x500000]=stoi(line.substr(ind1,ind2-ind1))&0x000000ff;++reg[28];
        mem[reg[28]-0x500000]=stoi(line.substr(ind1,ind2-ind1))&0x0000ff00;++reg[28];
    }
}

std::set<int> checkpoints;
void init_checkpoints(char* checkpoint_file) {
  FILE *fp=fopen(checkpoint_file,"r");
  int tmp,i=0;
  while(fscanf(fp,"%d",&tmp)!=EOF){
        checkpoints.insert(tmp);}}

void checkpoint_memory(int ins_count,vector<string>& codes) {
  if (!checkpoints.count(ins_count))return;
  std::string name="memory_"+std::to_string(ins_count)+".bin";
  FILE * fp =fopen(name.c_str(),"wb");
  for(int j=0;j<codes.size();j++){
    int code = twosComplement(codes.at(j));
    fwrite(&code,4,1,fp);}
  for (int j=codes.size()*4;j<0x100000;j++){
    int code = 0;
    fwrite(&code,1,1,fp);}
  for (int i=0;i<0x500000;i++){
    fwrite(&mem[i],1,1,fp);}
  fclose(fp);
  }

void checkpoint_register(int ins_count) {
  if (!checkpoints.count(ins_count))return;
  std::string name="register_"+std::to_string(ins_count)+".bin";
  FILE * fp=fopen(name.c_str(),"wb");
  int gp=0x508000;
  for (int i=0;i<32;i++){
    if(i==28){fwrite(&gp,4,1,fp);continue;}
    fwrite(&reg[i],4,1,fp);}
  fwrite(&pc,4,1,fp);
  fwrite(&hi,4,1,fp);
  fwrite(&lo,4,1,fp);
  fclose(fp);}

int main(int argc,char* argv[]) {
    reg[28]=0x500000;
    reg[29]=0xA00000;
    reg[30]=0xA00000;
    ifstream testFile;
    ifstream testin;
    ofstream testout;
    string line;
    vector<string> machineCodes;
    int exit = 0;
    testFile.open(argv[1]); //asm file
    while (getline(testFile,line)) {
        writeData(line);
        if (line.find(".text")!=-1)break;}
    testFile.close();
    testFile.open(argv[2]); //machine code file
    while (getline(testFile,line)){
        machineCodes.push_back(line);}
    testFile.close();
    testin.open(argv[4]);
    testout.open(argv[5]);
    init_checkpoints(argv[3]);
    while (exit==0 && ((pc-0x400000)/4<machineCodes.size())){ //execute machine codes
        checkpoint_memory((pc-0x400000)/4,machineCodes);
        checkpoint_register((pc-0x400000)/4);
        execute(machineCodes.at((pc-0x400000)/4),testin,testout,exit,machineCodes);
        pc=pc+4;}
    testin.close();
    testout.close();
}