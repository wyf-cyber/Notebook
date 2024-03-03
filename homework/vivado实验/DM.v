`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/12/02 12:35:48
// Design Name: 
// Module Name: dm
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

`define dm_word 3'b000
`define dm_halfword 3'b001
`define dm_byte 3'b011

module DM(
    input clk,  //100MHZ CLK
    input DMWr,  //write signal
    input [5:0]	addr,
    input [31:0] din,
    input [2:0]	DMType, 
    output reg [31:0] dout 
    );
    reg[7:0] dmem[127:0];
    integer i = 0;
    initial begin     
        for(i = 0; i < 128; i = i + 1) begin
            dmem[i] <= i;
        end
    end
    
    always@(posedge clk)begin
        if(DMWr == 1)begin
            case(DMType)//00字01半字11字节
                `dm_byte:dmem[addr] <= din[7:0];
                `dm_halfword:begin
                    dmem[addr] <= din[7:0];
                    dmem[addr+1] <= din[15:8];end
                `dm_word:begin
                    dmem[addr] <= din[7:0];
                    dmem[addr+1] <= din[15:8];
                    dmem[addr+2] <= din[23:16];
                    dmem[addr+3] <= din[31:24];end
            endcase
        end
    end
    
    always@(*)begin
            case(DMType)//00字01半字11字节
                `dm_byte:dout = {{24{dmem[addr][7]}},dmem[addr][7:0]};
                `dm_halfword:dout = {{16{dmem[addr+1][7]}},dmem[addr+1][7:0],dmem[addr][7:0]};
                `dm_word:dout = {dmem[addr+3][7:0],dmem[addr+2][7:0],dmem[addr+1][7:0],dmem[addr][7:0]};
                default:dout = dout;
            endcase
    end
endmodule