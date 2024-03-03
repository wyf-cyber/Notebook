`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/12/05 12:41:28
// Design Name: 
// Module Name: ctrl
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Control(
    input [6:0] Op,  //opcode
    input [6:0] fun7,  //fun7 
    input [2:0] fun3,    // fun3 
    output RegWrite, // control signal for register write
    output MemWrite, // control signal for memory write
    output	[5:0]EXTOp,    // control signal to signed extension
    output [4:0] ALUOp,    // ALU opertion
    output [2:0] branch,    // next pc operation
    output ALUSrc,   // ALU source for b
    output [2:0] DMType, //dm r/w type
    output [1:0] MemtoReg    // (register) write data selection  (MemtoReg)
    );
    //R_type:
    wire R_type  = ~Op[6]&Op[5]&Op[4]&~Op[3]&~Op[2]&Op[1]&Op[0]; //0110011  
    wire i_add   = R_type&~fun7[6]&~fun7[5]&~fun7[4]&~fun7[3]&~fun7[2]&~fun7[1]&~fun7[0]&~fun3[2]&~fun3[1]&~fun3[0]; // add 0000000 000
    wire i_sub   = R_type&~fun7[6]&fun7[5]&~fun7[4]&~fun7[3]&~fun7[2]&~fun7[1]&~fun7[0]&~fun3[2]&~fun3[1]&~fun3[0]; // sub 0100000 000
    //i_l type  
    wire itype_l  = ~Op[6]&~Op[5]&~Op[4]&~Op[3]&~Op[2]&Op[1]&Op[0]; //0000011
    wire i_lb=itype_l&~fun3[2]& ~fun3[1]& ~fun3[0]; //lb 000
    wire i_lh=itype_l&~fun3[2]& ~fun3[1]& fun3[0];  //lh 001
    wire i_lw=itype_l&~fun3[2]& fun3[1]& ~fun3[0];  //lw 010 
    // i_i type
    wire itype_r  = ~Op[6]&~Op[5]&Op[4]&~Op[3]&~Op[2]&Op[1]&Op[0]; //0010011
    wire i_addi  =  itype_r& ~fun3[2]& ~fun3[1]& ~fun3[0]; // addi 000 func3
    wire i_slti  =  itype_r&~fun3[2]& fun3[1]& ~fun3[0];// slti 010    0010011
  
    // s format
    wire stype  =  ~Op[6]&Op[5]&~Op[4]&~Op[3]&~Op[2]&Op[1]&Op[0];//0100011
    wire i_sw= stype& ~fun3[2]& fun3[1]&~fun3[0]; // sw 010
    wire i_sb=stype& ~fun3[2]& ~fun3[1]&~fun3[0];
    wire i_sh=stype&& ~fun3[2]&~fun3[1]&fun3[0];
    // j format
    wire jtype =  Op[6]&Op[5]&~Op[4]&Op[3]&Op[2]&Op[1]&Op[0];//1101111
    wire j_jal = jtype; // jal 
    // sb format
    wire sbtype =  Op[6]&Op[5]&~Op[4]&~Op[3]&~Op[2]&Op[1]&Op[0];//1100011
    wire sb_beq = sbtype&~fun3[2]&~fun3[1]&~fun3[0];  // fun3 000
    // jalr
    wire jalr =  Op[6]&Op[5]&~Op[4]&~Op[3]&Op[2]&Op[1]&Op[0]&~fun3[2]&~fun3[1]&~fun3[0];//1100111 fun3=000
    //操作指令生成控制信号（写、MUX选择）
    assign RegWrite   = R_type | itype_r|itype_l|jtype |jalr ; // register write
    assign MemWrite   = stype;            // memory write
    assign ALUSrc     = itype_r | stype | itype_l|jtype |jalr; // ALU B is from instruction immediate
    //MemtoReg_FromALU 2'b00  MemtoReg_FromMEM 2'b01 MemtoReg_FromPC 2'b10
    assign MemtoReg[0] = itype_l;   
    assign MemtoReg[1] = jtype | jalr;    

    //操作指令生成运算操作类型ALUOp
    //ALUOp_add  5'b00011
    //ALUOp_beq  5'b00100
    //ALUOp_sub  5'b00100
    //ALUOp_slti 5'b01000
    //ALUOp_jal  5'b10000
    //ALUOp_jalr 5'b00000
    assign ALUOp[0] = i_add  | i_addi|stype|itype_l ;
    assign ALUOp[1] = i_add  | stype|itype_l | i_addi;
    assign ALUOp[2] = sb_beq | i_sub;
    assign ALUOp[3] = i_slti;
    assign ALUOp[4] = j_jal;
    //操作指令生成常数扩展操作    
    // jalr 010000
    // jal  000001
    // beq  000100
    assign EXTOp[0] =  j_jal;
    assign EXTOp[2] =  sb_beq;
    assign EXTOp[3] =  stype;
    assign EXTOp[4] =  itype_l | itype_r | jalr; 
 
    //branch signal
    //nop - 00
    //jal - 01
    //jalr- 11
    //beq - 10
    assign branch[0] = j_jal | jalr;
    assign branch[1] = sb_beq| jalr;
    //根据具体S和i_L指令生成DataMem数据操作类型编码   
    // dm_word              3'b000
    // dm_halfword          3'b001
    // dm_halfword_unsigned 3'b010
    // dm_byte              3'b011
    // dm_byte_unsigned     3'b100
    assign DMType[2]= 1'b0;
    assign DMType[1]=i_lb | i_sb;
    assign DMType[0]=i_lh | i_sh | i_lb | i_sb;
endmodule
