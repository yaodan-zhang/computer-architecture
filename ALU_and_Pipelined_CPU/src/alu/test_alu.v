module test_alu();
    reg[31:0] instruction;
    reg[31:0] regA, regB;
    wire[31:0] result;
    wire[2:0] flags;
    alu dut(.instruction(instruction), .regA(regA), .regB(regB), .result(result), .flags(flags));
    //check instruction one at a time
    initial begin
        instruction=32'b00000000000000010000000000100000; regA=32'h7fffffff; regB=1; #10;//check add (overflow)
        if (result!=32'h80000000 || flags!=3'b001) $display("000 failed.");
        instruction=32'b00100000000000001111111111111110; regA=-1; regB=0; #10; //check addi
        if (result!=-3 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000100001; regA=-2; regB=3; #10; //check addu
        if (result!=1 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00100100001000001111111111111001; regA=0; regB=-8; #10; //check addiu
        if (result!=-15 ||flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000100010; regA=32'h7fffffff; regB=-1; #10; //check sub (overflow)
        if (result!=32'h80000000 || flags!=3'b001) $display("000 failed.");
        instruction=32'b00000000001000000000000000100011; regA=8; regB=4; #10; //check subu
        if(result!=-4 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000100100; regA=7;regB=9; #10; //check and
        if(result!=1 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00110000000000001111111111110000; regA=15;regB=16; #10;//check andi
        if(result!=0 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000100111; regA=32'hfffffff3; regB=4; #10;//check nor
        if(result!=8 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000100101; regA=3; regB=4; #10; //or
        if(result!=7 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00110100001000001111111100000000; regA=0; regB=15; #10;//ori
        if(result!=32'b11111111111111111111111100001111 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000100110; regA=7; regB=8; #10; //xor
        if(result!=15 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00111000000000001111111111111111; regA=31; regB=0; #10;//xori
        if(result!=32'b00000000000000001111111111100000 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00010000000000010000000000000000; regA=-6; regB=-6;#10;//beq
        if(flags!=3'b100) $display("000 failed.");
        instruction=32'b00010100000000010000000000000000; regA=2;regB=0;#10;//bne
        if(flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000101010; regA=-1;regB=0;#10;//slt
        if(flags!=3'b010) $display("000 failed.");
        instruction=32'b00101000001000001111111111111111; regA=0; regB=-2;#10;//slti
        if(flags!=3'b010) $display("000 failed.");
        instruction=32'b00101100000000001111111111111111; regA=0; regB=0;#10;//sltiu
        if(flags!=3'b010) $display("000 failed.");
        instruction=32'b00000000000000010000000000101011; regA=-1; regB=0;#10;//sltu
        if(flags!=3'b000) $display("000 failed.");
        instruction=32'b10001100000000001111111111110000; regA=15; regB=0;#10;//lw
        if(result!=32'h0000ffff || flags!=3'b000) $display("000 failed.");
        instruction=32'b10101100001000000000000000001111; regA=0;regB=32'hfffffff0;#10;//sw
        if(result!=32'hffffffff || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000010000000; regA=0;regB=2;#10;//sll
        if(result!=8 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000000100; regA=2;regB=16;#10;//sllv
        if(result!=64 || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000000000000100000010; regA=32'hffffffff;regB=0;#10;//srl
        if(result!=32'h0fffffff || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000001000000000000000000110; regA=32'hffffffff;regB=4;#10;//srlv
        if(result!=32'h0fffffff || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000100000011; regA=0;regB=32'hfffffff0;#10;//sra
        if(result!=32'hffffffff || flags!=3'b000) $display("000 failed.");
        instruction=32'b00000000000000010000000000000111; regA=8;regB=32'hffff0000;#10;//srav
        if(result!=32'hffffff00 || flags!=3'b000) $display("000 failed.");
        $display("test done!");
    end
endmodule