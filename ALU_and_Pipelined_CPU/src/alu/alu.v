// instruction: 32-bit instruction
// regA/B: 32-bit data in registerA(addr=00000), registerB(addr=00001)
// result: 32-bit result of Alu execution
// flags: 3-bit alu flag
// flags[2] : zero flag
// flags[1] : negative flag
// flags[0] : overflow flag
module alu(input[31:0] instruction,
    input[31:0] regA,regB,
    output reg[31:0] result,
    output reg[2:0] flags);
    reg[5:0] opcode, func;
    reg[31:0] reg0,reg1;
    reg signed[31:0] reg2,reg3;
    always @(*)begin
        opcode = instruction[31:26];
        func = instruction[5:0];
        flags = 3'b000;
        if(opcode==6'b000000 && func==6'b100000) begin //add
            result = regA+regB;
            if(!regA[31]&&!regB[31]&&result[31] || regA[31]&&regB[31]&&!result[31])
                flags[0]=1;
        end
        else if(opcode==6'b000000 && func==6'b100001) begin //addu
            result=regA+regB;
        end
        else if(opcode==6'b000000 && func==6'b100010) begin //sub
            if(instruction[20:16]==5'b00001 && instruction[25:21]==5'b00000)begin
                reg2=$signed(-regB);
                result=regA+reg2;
                if(!regA[31]&&!reg2[31]&&result[31] || regA[31]&&reg2[31]&&!result[31])
                flags[0]=1;
            end
            if(instruction[20:16]==5'b00000 && instruction[25:21]==5'b00001) begin
                reg2=$signed(-regA);
                result=regB+reg2;
                if(!regB[31]&&!reg2[31]&&result[31] || regB[31]&&reg2[31]&&!result[31])
                flags[0]=1;
            end
        end
        else if(opcode==6'b000000 && func==6'b100011) begin //subu
            if(instruction[20:16]==5'b00001 && instruction[25:21]==5'b00000)begin
                result=regA-regB;
            end
            if(instruction[20:16]==5'b00000 && instruction[25:21]==5'b00001)begin
                result=regB-regA;
            end
        end
        else if(opcode==6'b000000 && func==6'b100100) begin //and
            result=regA&regB;
        end
        else if(opcode==6'b000000 && func==6'b100111) begin //nor
            result=~(regA|regB);
        end
        else if(opcode==6'b000000 && func==6'b100101) begin //or
            result=regA|regB;
        end
        else if(opcode==6'b000000 && func==6'b100110) begin //xor
            result=regA^regB;
        end
        else if(opcode==6'b000000 && func==6'b101010) begin //slt
            reg2=$signed(regA);
            reg3=$signed(regB);
            if(instruction[20:16]==5'b00000 && instruction[25:21]==5'b00001)begin
                result=reg3-reg2;
                if(reg3<reg2)
                    flags[1]=1;
            end
            else begin
                result=$signed(reg2-reg3);
                if(reg2<reg3)
                    flags[1]=1;
            end
        end
        else if(opcode==6'b000000 && func==6'b101011) begin //sltu
            if(instruction[20:16]==5'b00000 && instruction[25:21]==5'b00001)begin
                result=regB-regA;
                if(regB<regA)
                    flags[1]=1;
            end
            else begin
                result=regA-regB;
                if(regA<regB)
                    flags[1]=1;
            end
        end
        else if(opcode==6'b000000 && func==6'b000000) begin //sll
            reg0=instruction[10:6];
            if(instruction[20:16]==5'b00000)
                result=regA<<reg0;
            if(instruction[20:16]==5'b00001)
                result=regB<<reg0;
        end
        else if(opcode==6'b000000 && func==6'b000100) begin //sllv
            if(instruction[20:16]==5'b00000 && instruction[25:21]==5'b00001)
                result=regA<<regB;
            if(instruction[20:16]==5'b00001 && instruction[25:21]==5'b00000)
                result=regB<<regA;
        end
        else if(opcode==6'b000000 && func==6'b000010) begin //srl
            reg0=instruction[10:6];
            if(instruction[20:16]==5'b00000)
                result=regA>>reg0;
            if(instruction[20:16]==5'b00001)
                result=regB>>reg0;
        end
        else if(opcode==6'b000000 && func==6'b000110) begin //srlv
            if(instruction[20:16]==5'b00000 && instruction[25:21]==5'b00001)
                result=regA>>regB;
            if(instruction[20:16]==5'b00001 && instruction[25:21]==5'b00000)
                result=regB>>regA;
        end
        else if(opcode==6'b000000 && func==6'b000011) begin //sra
            reg0=instruction[10:6];
            if(instruction[20:16]==5'b00000)
                result=($signed(regA))>>>reg0;
            if(instruction[20:16]==5'b00001)
                result=($signed(regB))>>>reg0;
        end
        else if(opcode==6'b000000 && func==6'b000111) begin //srav
            if(instruction[20:16]==5'b00000 && instruction[25:21]==5'b00001)
                result=($signed(regA))>>>regB;
            if(instruction[20:16]==5'b00001 && instruction[25:21]==5'b00000)
                result=($signed(regB))>>>regA;
        end
        else if(opcode==6'b001000) begin //addi
            reg2=$signed(instruction[15:0]);
            if(instruction[25:21]==5'b00000) begin
                result=regA+reg2;
                if(!regA[31]&&!reg2[31]&&result[31] || regA[31]&&reg2[31]&&!result[31])
                flags[0]=1;
            end
            if(instruction[25:21]==5'b00001) begin
                result=regB+reg2;
                if(!regB[31]&&!reg2[31]&&result[31] || regB[31]&&reg2[31]&&!result[31])
                flags[0]=1;
            end
        end
        else if(opcode==6'b001001) begin //addiu
            reg2=$signed(instruction[15:0]);
            if(instruction[25:21]==5'b00000)
                result=regA+reg2;
            if(instruction[25:21]==5'b00001)
                result=regB+reg2;
        end
        else if(opcode==6'b001100) begin //andi
            reg0=$signed(instruction[15:0]);
            if(instruction[25:21]==5'b00000)
                result=regA&reg0;
            if(instruction[25:21]==5'b00001)
                result=regB&reg0;
        end
        else if(opcode==6'b001101) begin //ori
            reg0=$signed(instruction[15:0]);
            if(instruction[25:21]==5'b00000)
                result=regA|reg0;
            if(instruction[25:21]==5'b00001)
                result=regB|reg0;
        end
        else if(opcode==6'b001110) begin //xori
            reg0=instruction[15:0];
            if(instruction[25:21]==5'b00000)
                result=regA^reg0;
            if(instruction[25:21]==5'b00001)
                result=regB^reg0;
        end
        else if(opcode==6'b000100) begin //beq
            result=regA-regB;
            if(result==0)
                flags[2]=1;
        end
        else if(opcode==6'b000101) begin //bne
            result=regA-regB;
            if(result==0)
                flags[2]=1;
        end
        else if(opcode==6'b001010) begin //slti
            reg2=$signed(instruction[15:0]);
            if(instruction[25:21]==5'b00000)begin
                result=regA-reg2;
                if(regA<reg2)
                    flags[1]=1;
            end
            if(instruction[25:21]==5'b00001)begin
                result=regB-reg2;
                if(regB<reg2)
                    flags[1]=1;
            end
        end
        else if(opcode==6'b001011) begin //sltiu
            reg0=instruction[15:0];
            if(instruction[25:21]==5'b00000)begin
                result=regA-reg0;
                if(regA<reg0)
                    flags[1]=1;
            end
            if(instruction[25:21]==5'b00001)begin
                result=regB-reg0;
                if(regB<reg0)
                    flags[1]=1;
            end
        end
        else if(opcode==6'b100011) begin //lw
            reg2=instruction[15:0];
            if(instruction[25:21]==5'b00000)
                result=regA+reg2;
            if(instruction[25:21]==5'b00001)
                result=regB+reg2;
        end
        else if(opcode==6'b101011) begin //sw
            reg2=instruction[15:0];
            if(instruction[25:21]==5'b00000)
                result=regA+reg2;
            if(instruction[25:21]==5'b00001)
                result=regB+reg2;
        end
    end
endmodule

