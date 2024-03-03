`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/19 09:14:33
// Design Name: 
// Module Name: alu
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

`define ALUOp_add 5'b00011
`define ALUOp_sub 5'b00100
`define ALUOp_slti 5'b01000
`define ALUOp_jal 5'b10000
`define ALUOp_jalr 5'b00000
module ALU(
    input signed[31:0] A,B,
    input[4:0] ALUOp,
    output  reg signed[31:0] C,
    output [7:0] Zero
    );
    reg[3:0] offset = 4'b0;
    always@(*)begin
        case(ALUOp)
            `ALUOp_add:C = A + B;
            `ALUOp_sub:C = A - B;
            `ALUOp_slti: C = (A < B) ? 32'b1 : 32'b0; 
            `ALUOp_jal: C = A + (B >>> 2);   // 因为一条指令占4个字节所以还需要除4
            `ALUOp_jalr: C = (A + B) >>> 2;   // 因为一条指令占4个字节所以还需要除4
        endcase
    end
    assign Zero = (C == 0)? 1:0;
endmodule