module IF(input CLOCK,jump2,branch2,input[31:0]im,output reg[31:0]instruction,curpc);
  integer fd,c1,c2;
  integer pc=0;
  always @(posedge CLOCK)begin
    fd = $fopen("CPU_instruction.bin", "rb");
    if(branch2==1'b1)begin
      pc=(pc/33+im-1)*33;
    end
    if(jump2==1'b1)begin
      pc=im*0.25*33;
    end
    c1=$fseek(fd, pc, 0);
    c2=$fscanf(fd, "%b", instruction);
    $fclose(fd);
    pc=pc+33;
    curpc=pc/33;//current pc position
  end
endmodule
module REG(input CLOCK,memread,memwrite,jump,branch,R,I,input[31:0]instruction,data,loc,offset,curpc,rtloc2,rdloc2,rsloc2,res,im2,rs2,output reg[5:0] opcode,funcode,output reg[31:0] rd,rt,rs,im,zeroim,tar,sa,rtloc,rdloc,rsloc,curpc2,output reg jump2,branch2);
  reg[31:0] register[31:0];
  integer j,a1,a2;
  initial begin
    for (j=0;j<32;j=j+1)begin
      register[j]=0;
    end
  end
  always @(posedge CLOCK) begin
    if(loc==32'hffffffff)begin
      $finish;
    end
    register[loc]=data;//RegW
  end
  always @(negedge CLOCK) begin
    jump2=1'b0;
    branch2=1'b0;
    opcode = instruction[31:26];
    funcode = instruction[5:0];
    sa = instruction[10:6];
    im = $signed(instruction[15:0]);
    zeroim = instruction[15:0];
    tar = instruction[25:0];
    rd = register[instruction[15:11]];//RegR
    rt = register[instruction[20:16]];
    rs = register[instruction[25:21]];
    rdloc=instruction[15:11];
    rtloc=instruction[20:16];
    rsloc=instruction[25:21];
    curpc2=curpc;
    if(I==1'b1 && instruction[25:21]==rtloc2 && funcode==6'b001000 && memread==1'b0 && branch==1'b0)begin
      rs=res;
    end
    if(I==1'b1 && instruction[25:21]==rtloc2 && funcode==6'b001000 && memread==1'b1)begin
      a1 = $fopen("data.bin","rb");
      a2 = $fseek(a1,(im2+rs2)*0.25*33,0);
      a2 = $fscanf(a1,"%b",rs);
      $fclose(a1);
    end
    if(opcode==6'b000000 && funcode==6'b100010 && R==1'b1 && rsloc==rdloc2)begin
      rs=res;
    end
    if(opcode==6'b101011 && rtloc==rdloc2 && R==1'b1)begin
      rt=res;
    end
    if(opcode==6'b000101 && rsloc==rdloc2 && R==1'b1)begin
      rs=res;
    end
    if(opcode==6'b001000 && rsloc==rtloc2 && I==1'b1 && memread==1'b0)begin
      rs=res;
    end
    if(jump==1'b1)begin
      opcode=6'b111111;
      im=offset;
      jump2=1'b1;
    end
    if(branch==1'b1)begin
      opcode=6'b111111;
      im=offset;
      branch2=1'b1;
    end
  end
endmodule
module ALU(input CLOCK,memread2,input[5:0]opcode,funcode,input[31:0]rd,rt,rs,sa,im,zeroim,tar,rtloc,rsloc,rdloc,curpc2,data,rtloc3,output reg[31:0] res,offset,loc1,loc2,rtloc2,rsloc2,rdloc2,im2,rs2,output reg memread,memwrite,jump,branch,termi,R,I);
  always @(posedge CLOCK) begin
    memread=1'b0;
    memwrite=1'b0;//reset
    jump=1'b0;
    branch=1'b0;
    offset=32'h00000000;
    termi=1'b0;
    rdloc2=rdloc;
    rtloc2=rtloc;
    rsloc2=rsloc;
    im2=im;
    rs2=rs;
    if (opcode==6'b000000)begin
      R=1'b1;
    end
    if (opcode!=6'b000000&&opcode!=6'b000010&&opcode!=6'b000011)begin
      I=1'b1;
    end
    if((opcode==6'b000000 && funcode==6'b100000)||(opcode==6'b000000 && funcode==6'b100001))begin
      res=rs+rt;
      loc2=rdloc;
    end
    if((opcode==6'b000000 && funcode==6'b100010)||(opcode==6'b000000 && funcode==6'b100011))begin
      if(memread2==1'b1 && rtloc3==rtloc)begin
        res=$signed(rs-data); 
      end
      else begin
        res=$signed(rs-rt);
      end
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b100100)begin
      res=rs&rt;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b100111)begin
      res=~(rs|rt);
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b100101)begin
      res=rs|rt;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b100110)begin
      res=rs^rt;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b000000)begin
      res=rt<<sa;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b000100)begin
      res=rt<<rs;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b000010)begin
      res=rt>>sa;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b000110)begin
      res=rt>>rs;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b000011)begin
      res=($signed(rt))>>>sa;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b000111)begin
      res=($signed(rt))>>>rs;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b101010)begin
      if(rs<rt)
        res=1;
      if(rs>=rt)
        res=0;
      loc2=rdloc;
    end
    if(opcode==6'b000000 && funcode==6'b001000)begin//jr
      if(memread2==1'b1 && rsloc==rtloc3)begin//lw stall
        offset=data;
        jump=1'b1;
      end
      else begin
        offset=rs;
        jump=1'b1;
      end
    end
    if(opcode==6'b001000||opcode==6'b001001)begin
      res=rs+im;
      loc2=rtloc;
    end
    if(opcode==6'b001100)begin
      res=rs&zeroim;
      loc2=rtloc;
    end
    if(opcode==6'b001101)begin
      res=rs|zeroim;
      loc2=rtloc;
    end
    if(opcode==6'b001110)begin
      res=rs^zeroim;
      loc2=rtloc;
    end
    if(opcode==6'b000100)begin//beq
      if(memread2==1'b1 && rsloc==rtloc3)begin//lw stall
        if(data==rt)begin
          offset=im;
          branch=1'b1;
        end
      end
      else begin//no lw stall
        if(rs==rt)begin
          offset=im;
          branch=1'b1;
        end
      end
    end
    if(opcode==6'b100011)begin
      memread=1'b1;
      loc1=(im+rs)*0.25;
      loc2=rtloc;
    end
    if(opcode==6'b101011)begin
      memwrite=1'b1;
      loc1=(im+rs)*0.25;
      res=rt;
    end
    if(opcode==6'b000101 && rs!=rt)begin
      offset=im;
      branch=1'b1;//bne
    end
    if(opcode==6'b000010)begin
      offset=tar<<2;
      jump=1'b1;//j
    end
    if(opcode==6'b000011)begin//jal
      offset=tar<<2;
      res=curpc2<<2;
      loc2=31;
      jump=1'b1;
    end
    if(opcode==6'b111111 && funcode==6'b111111)begin//exit sign
      termi=1'b1;
    end
  end
endmodule
module MeM(input CLOCK,memread,memwrite,termi,input[31:0]loc1,loc2,res,rtloc2,output reg[31:0]data,loc,rtloc3,output reg memread2);
  integer i,j,b1;
  reg[31:0] memory[511:0];
  initial begin
    for(j=0;j<512;j=j+1)begin
      memory[j]=0;
    end
  end
  always @(negedge CLOCK) begin
    if(memread==1'b1)begin//lw
      rtloc3=rtloc2;
      memread2=memread;
      data=memory[loc1];
      loc = loc2;
    end
    if(memwrite==1'b1)begin//sw
      memory[loc1]=res;
    end
    if(memread==1'b0 && memwrite==1'b0)begin
      data=$signed(res);
      loc=loc2;
    end
    if(termi==1'b1)begin
      loc=32'hffffffff;
      b1 = $fopen("data.bin","wb");
      for (i=0;i<512;i=i+1)begin
        $fdisplay(b1,"%b",memory[i]);
      end
      $fclose(b1);
    end
  end
endmodule

module PipelinedCPU(input CLK);
  wire[31:0] INSTRUCTION,DATA,RD,RT,RS,IM,ZEROIM,TAR,RES,LOC1,LOC2,LOC,OFFSET,SA,RTLOC,RDLOC,PC,PC2,RTLOC2,RSLOC2,RDLOC2,IM2,RS2,RSLOC,RTLOC3;
  wire[5:0] OPCD,FUCD;
  wire MEMR,MEMW,JP,BCH,TERMI,JP2,BCH2,R,I,MEMR2;
  IF u_IF(
    .CLOCK(CLK),
    .im(IM),
    .jump2(JP2),
    .branch2(BCH2),
    .instruction(INSTRUCTION),
    .curpc(PC));
  REG u_REG(
    .CLOCK(CLK),
    .memread(MEMR),
    .memwrite(MEMW),
    .loc(LOC),
    .data(DATA),
    .instruction(INSTRUCTION),
    .jump(JP),
    .branch(BCH),
    .R(R),
    .I(I),
    .offset(OFFSET),
    .curpc(PC),
    .rdloc2(RDLOC2),
    .rtloc2(RTLOC2),
    .rsloc2(RSLOC2),
    .res(RES),
    .im2(IM2),
    .rs2(RS2),
    .opcode(OPCD),
    .funcode(FUCD),
    .rd(RD),
    .rt(RT),
    .rs(RS),
    .sa(SA),
    .im(IM),
    .zeroim(ZEROIM),
    .tar(TAR),
    .rtloc(RTLOC),
    .rdloc(RDLOC),
    .rsloc(RSLOC),
    .curpc2(PC2),
    .jump2(JP2),
    .branch2(BCH2));
  ALU u_ALU(
    .CLOCK(CLK),
    .memread2(MEMR2),
    .opcode(OPCD),
    .funcode(FUCD),
    .rd(RD),
    .rt(RT),
    .rs(RS),
    .sa(SA),
    .im(IM),
    .zeroim(ZEROIM),
    .tar(TAR),
    .rtloc(RTLOC),
    .rdloc(RDLOC),
    .rsloc(RSLOC),
    .curpc2(PC2),
    .data(DATA),
    .rtloc3(RTLOC3),
    .res(RES),
    .offset(OFFSET),
    .memread(MEMR),
    .memwrite(MEMW),
    .loc1(LOC1),
    .loc2(LOC2),
    .rdloc2(RDLOC2),
    .rsloc2(RSLOC2),
    .rtloc2(RTLOC2),
    .im2(IM2),
    .rs2(RS2),
    .jump(JP),
    .branch(BCH),
    .termi(TERMI),
    .R(R),
    .I(I));
  MeM u_MeM(
    .CLOCK(CLK),
    .loc1(LOC1),
    .loc2(LOC2),
    .res(RES),
    .memread(MEMR),
    .memwrite(MEMW),
    .termi(TERMI),
    .rtloc2(RTLOC2),
    .data(DATA),
    .loc(LOC),
    .rtloc3(RTLOC3),
    .memread2(MEMR2));
endmodule