#include <string>
#include "LabelTable.cpp"
using namespace std;

const string R[27] = {"add","addu","and","div","divu","jalr","jr","mfhi","mflo","mthi","mtlo","mult",
"multu","nor","or","sll","sllv","slt","sltu","sra","srav","srl","srlv","sub","subu","syscall","xor"};

const string I[26] = {"addi","addiu","andi","beq","bgez","bgtz","blez","bltz","bne","lb","lbu","lh",
"lhu","lui","lw","ori","sb","slti","sltiu","sh","sw","xori","lwl","lwr","swl","swr"};

const string J[2] = {"j", "jal"};

string operandCode(string oprd) {
    string regNum;
    if (oprd == "zero") {regNum = "00000";}
    else if (oprd == "at") {regNum = "00001";}
    else if (oprd == "v0") {regNum = "00010";}
    else if (oprd == "v1") {regNum = "00011";}
    else if (oprd == "a0") {regNum = "00100";}
    else if (oprd == "a1") {regNum = "00101";}
    else if (oprd == "a2") {regNum = "00110";}
    else if (oprd == "a3") {regNum = "00111";}
    else if (oprd == "t0") {regNum = "01000";}
    else if (oprd == "t1") {regNum = "01001";}
    else if (oprd == "t2") {regNum = "01010";}
    else if (oprd == "t3") {regNum = "01011";}
    else if (oprd == "t4") {regNum = "01100";}
    else if (oprd == "t5") {regNum = "01101";}
    else if (oprd == "t6") {regNum = "01110";}
    else if (oprd == "t7") {regNum = "01111";}
    else if (oprd == "s0") {regNum = "10000";}
    else if (oprd == "s1") {regNum = "10001";}
    else if (oprd == "s2") {regNum = "10010";}
    else if (oprd == "s3") {regNum = "10011";}
    else if (oprd == "s4") {regNum = "10100";}
    else if (oprd == "s5") {regNum = "10101";}
    else if (oprd == "s6") {regNum = "10110";}
    else if (oprd == "s7") {regNum = "10111";}
    else if (oprd == "t8") {regNum = "11000";}
    else if (oprd == "t9") {regNum = "11001";}
    else if (oprd == "k0") {regNum = "11010";}
    else if (oprd == "k1") {regNum = "11011";}
    else if (oprd == "gp") {regNum = "11100";}
    else if (oprd == "sp") {regNum = "11101";}
    else if (oprd == "fp") {regNum = "11110";}
    else if (oprd == "ra") {regNum = "11111";}
    return regNum;
}

string addTwoBinarys(string bin1, string bin2) {
    string addRes = "";
    int carry = 0;
    int len = bin1.length() - 1;
    int re;
    for (int i = len; i >= 0; i--) {
       re = carry;
       if(bin1[i] == '1') {re = re+1;}
       else {re = re+0;}
       if(bin2[i] == '1') {re = re+1;}
       else {re = re+0;}
       if(re%2==1) {addRes = "1" + addRes;}
       else {addRes = "0" + addRes;}
       if(re<2) {carry = 0;}
       else {carry = 1;}
    }
    if(carry != 0) {addRes = "1" + addRes;}
    return addRes;
}

string sixteenTwosComplement(string decimal) { //16 digit 2's complement form
    if (decimal[0] == '-') {
        string minus = "";
        string binary = sixteenTwosComplement(decimal.substr(1));
        for (int i = 15; i >= 0; i--) {
            if (binary[i] == '0') {minus = "1" + minus;}
            else if (binary[i] == '1') {minus = "0" + minus;}
        }
        return addTwoBinarys(minus, "0000000000000001");
    } else {
        int binaryNum[16] = {0};
        int num = stol(decimal);
        int i = 0;
        string binary = "";
        while (num > 0) {
            binaryNum[i] = num % 2;
            num = num / 2;
            i++;
        }
        for (int m = 15; m >= 0; m--) {
            binary = binary + to_string(binaryNum[m]);
        }
        return binary;
    }
}

string toBinaryNumber(string decimal) { //5 digit binary number, used for sa in R type
    int binaryNum[5] = {0,0,0,0,0};
    int num = stol(decimal);
    string binary = "";
    int i = 0;
    while (num > 0 && i<=5) {
        binaryNum[i] = num % 2;
        num = num / 2;
        i++; }
    for (int j = 4; j>=0; j--) { binary = binary + std::to_string(binaryNum[j]); }
    return binary;
}

string addressoffset(string label, int k) { //16 digit address offset (-4), drop last 2 digit
    string labelAddress = findValue(label).at(1);
    return sixteenTwosComplement(to_string(stoi(labelAddress)-k-1));
}

string findOperand(string line, int ind) {
    string operand = line.substr(ind, line.find(",", ind) - ind);
    return operandCode(operand);
}

string JTypeCode(int index, string line) {
    string opcode, label;
    switch (index) {
        case 0: 
            opcode = "000010";
            label = line.substr(1);
            break;
        case 1:
            opcode = "000011";
            label = line.substr(3);
            break;
        default: break;
    }
    return opcode + findValue(label).at(0).substr(0,26);
}

string ITypeCode (int index, string line, int k) {
    string opcode, rs, rt, im, label;
    if (index == 0||index == 1||index == 2||index == 15||index == 17||index == 18||index == 21) {
        switch (index) {
            case 0: opcode = "001000"; break;
            case 1: opcode = "001001"; break;
            case 2: opcode = "001100"; break;
            case 15: opcode = "001101"; break;
            case 17: opcode = "001010"; break;
            case 18: opcode = "001011"; break;
            case 21: opcode = "001110"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rt = findOperand(line, found);
        found = line.find("$", found) + 1;
        rs = findOperand(line, found);
        int pos = line.find(",", found) + 1;
        im = line.substr(pos);
        return opcode + rs + rt + sixteenTwosComplement(im);
    
    } else if (index == 3||index == 8) {
        switch (index) {
            case 3: opcode = "000100"; break;
            case 8: opcode = "000101"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rs = findOperand(line, found);
        found = line.find("$", found) + 1;
        rt = findOperand(line, found);
        int pos = line.find(",", found) + 1;
        label = line.substr(pos);
        return opcode + rs + rt + addressoffset(label, k);
    
    } else if (index == 4||index == 5||index == 6||index == 7) {
        switch (index) {
            case 4: opcode = "000001"; rt = "00001"; break;
            case 5: opcode = "000111"; rt = "00000"; break;
            case 6: opcode = "000110"; rt = "00000"; break;
            case 7: opcode = "000001"; rt = "00000"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rs = findOperand(line, found);
        int pos = line.find(",", found) + 1;
        label = line.substr(pos);
        return opcode + rs + rt + addressoffset(label, k);
    
    } else if (index == 9||index == 10||index == 11||index == 12||index == 14||index == 16||
            index == 19||index == 20||index == 22||index == 23||index == 24||index == 25) {
        switch (index) {
            case 9: opcode = "100000"; break;
            case 10: opcode = "100100"; break;
            case 11: opcode = "100001"; break;
            case 12: opcode = "100101"; break;
            case 14: opcode = "100011"; break;
            case 16: opcode = "101000"; break;
            case 19: opcode = "101001"; break;
            case 20: opcode = "101011"; break;
            case 22: opcode = "100010"; break;
            case 23: opcode = "100110"; break;
            case 24: opcode = "101010"; break;
            case 25: opcode = "101110"; break; 
            default: break;
        }
        int found = line.find("$") + 1;
        rt = findOperand(line, found);
        int pos = line.find(",", found) + 1;
        im = line.substr(pos, line.find("(", pos) - pos);
        rs = line.substr(line.find("(", pos) + 2, line.find(")", pos) - line.find("(", pos) - 2);
        return opcode + operandCode(rs) + rt + sixteenTwosComplement(im);
    
    } else if (index == 13) {
        opcode = "001111";
        int found = line.find("$") + 1;
        rt = findOperand(line, found);
        rs = "00000";
        int pos = line.find(",", found) + 1;
        im = line.substr(pos);
        return opcode + rs + rt + sixteenTwosComplement(im);
    }
    return NULL;
}

string RTypeCode (int index, string line) {
    string opcode = "000000";
    string funcode, rd, rs, rt, sa, shift;
    if (index == 0||index == 1||index == 2||index == 13||index == 14||index == 17||
        index == 18||index == 23||index == 24||index == 26) {
        switch (index) {
            case 0: funcode = "100000"; break;
            case 1: funcode = "100001"; break;
            case 2: funcode = "100100"; break;
            case 13: funcode = "100111"; break;
            case 14: funcode = "100101"; break;
            case 17: funcode = "101010"; break;
            case 18: funcode = "101011"; break;
            case 23: funcode = "100010"; break;
            case 24: funcode = "100011"; break;
            case 26: funcode = "100110"; break;
            default: break; 
        }
        int found = line.find("$") + 1;
        rd = findOperand(line, found);
        found = line.find("$", found) + 1;
        rs = findOperand(line, found);
        found = line.find("$", found) + 1;
        rt = findOperand(line, found);
        sa = "00000";
        return opcode + rs + rt + rd + sa + funcode;
    
    } else if (index == 16||index == 20||index == 22) {
        switch (index) {
            case 16: funcode = "000100"; break;
            case 20: funcode = "000111"; break;
            case 22: funcode = "000110"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rd = findOperand(line, found);
        found = line.find("$", found) + 1;
        rt = findOperand(line, found);
        found = line.find("$", found) + 1;
        rs = findOperand(line, found);
        sa = "00000";
        return opcode + rs + rt + rd + sa + funcode;
    
    } else if (index == 3||index == 4||index == 11||index == 12) {
        switch (index) {
            case 3: funcode = "011010"; break;
            case 4: funcode = "011011"; break;
            case 11: funcode = "011000"; break;
            case 12: funcode = "011001"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rs = findOperand(line, found);
        found = line.find("$", found) + 1;
        rt = findOperand(line, found);
        rd = "00000";
        sa = "00000";
        return opcode + rs + rt + rd + sa + funcode;
    
    } else if (index == 5) {
        funcode = "001001";
        int found = line.find("$") + 1;
        rd = findOperand(line, found);
        found = line.find("$", found) + 1;
        rs = findOperand(line, found);
        rt = "00000";
        sa = "00000";
        return opcode + rs + rt + rd + sa + funcode;
    
    } else if (index == 6||index == 9||index == 10) {
        switch (index) {
            case 6: funcode = "001000"; break;
            case 9: funcode = "010001"; break;
            case 10: funcode = "010011"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rs = findOperand(line, found);
        rt = "00000";
        rd = "00000";
        sa = "00000";
        return opcode + rs + rt + rd + sa + funcode;

    } else if (index == 7||index == 8) {
        switch (index) {
            case 7: funcode = "010000"; break;
            case 8: funcode = "010010"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rd = findOperand(line, found);
        rs = "00000";
        rt = "00000";
        sa = "00000";
        return opcode + rs + rt + rd + sa + funcode;

    } else if (index == 15||index == 19||index == 21) {
        switch (index) {
            case 15: funcode = "000000"; break;
            case 19: funcode = "000011"; break;
            case 21: funcode = "000010"; break;
            default: break;
        }
        int found = line.find("$") + 1;
        rd = findOperand(line, found);
        found = line.find("$", found) + 1;
        rt = findOperand(line, found);
        int pos = line.find(",", found) + 1;
        shift = line.substr(pos);
        rs = "00000";
        sa = toBinaryNumber(shift);
        return opcode + rs + rt + rd + sa + funcode;
    
    } else if (index == 25) {
        funcode = "001100";
        rs = "00000";
        rt = "00000";
        rd = "00000";
        sa = "00000";
        return opcode + rs + rt + rd + sa + funcode;
    }
    return NULL;
}

int isInArray(const string arr[], string item, int size) {
    for(int i = 0; i < size; i++){
        if(arr[i] == item){
            return i;
        }
    }
    return -1;
}

string GetMachineCode(string line, int k) {
    string operation;
    int ind = line.find("$");
    if (ind == -1){
        if (line.substr(0,3) == "jal") {operation = "jal";}
        else if (line.substr(0,1) == "j") {operation = "j";}
        else {operation = "syscall";}
    }
    else {operation = line.substr(0,ind);}
    if (isInArray(R, operation, 27) != -1) {    
        return RTypeCode(isInArray(R, operation, 27), line);
    } else if (isInArray(I, operation, 26) != -1) {
        return ITypeCode(isInArray(I, operation, 26), line, k);
    } else if (isInArray(J, operation, 2) != -1) {
        return JTypeCode(isInArray(J, operation, 2), line);
    }
    return NULL;
}

