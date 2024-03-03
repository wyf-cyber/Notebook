`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/05 09:38:52
// Design Name: 
// Module Name: SCPU_TOP
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

//beq_test code:00332193 00632093 00732113 ffe32213 fe208ae3 fe3088e3 00528033
//slti x3,x6,3
//label:
//slti x1,x6,6
//slti x2,x6,7
//slti x4,x6,-2
//beq x1,x2,label
//beq x1,x3,label
//add x0,x5,x5

//jal_test code:00332193 00632093 00732113 ffe32213 ff5ff46f 00528033
//slti x3,x6,3
//label:
//slti x1,x6,6
//slti x2,x6,7
//slti x4,x6,-2
//jal x8,label
//add x0,x5,x5

//jalr_test code:00332193 00000293 00428467 00632093 00732113 ffe32213 00528033
//slti x3,x6,3
//addi x5,x0,0
//jalr x8,4(x5)
//slti x1,x6,6
//slti x2,x6,7
//slti x4,x6,-2
//add x0,x5,x5


`define MemtoReg_FromALU 2'b00
`define MemtoReg_FromMEM 2'b01
`define MemtoReg_FromPC 2'b10
module CPU(
    input clk,  //100MHZ CLK
    input rstn,  //reset signal
    input [15:0] sw_i, //sw_i[15]---sw_i[0] 
    output [7:0] disp_an_o, //8位数码管位选
    output [7:0] disp_seg_o //数码管8段数据
    );
    
    reg[31:0] clkdiv;
    wire Clk_CPU;
    //分频
    always @(posedge clk or negedge rstn) begin
        if(!rstn) clkdiv <= 0;
        else clkdiv <= clkdiv + 1'b1; 
    end
    //sw_i[15]选择慢速模式或快速模式
    assign Clk_CPU = (sw_i[15])? clkdiv[27] : clkdiv[25];
    
    reg[4:0]rom_addr = 5'b11111;
    wire[31:0] inst_in;
    wire[2:0] branch;
    //rom每周期地址顺序取数
    //更新PC地址的代码逻辑全部放置于WB阶段
 
    //从coe文件中取指令
    dist_instr u_im(
        .a(rom_addr),
        .spo(inst_in)
    );
    
    reg[6:0] Op;
    reg[6:0] fun7;
    reg[2:0] fun3;
    reg[11:0] iimm;
    reg[11:0] simm;
    reg[19:0] jimm;
    reg[11:0] bimm;
    wire[1:0] MemtoReg;
    wire[5:0] EXTOp;
    reg[4:0]rs1;
    reg[4:0]rs2;
    reg[4:0]rd;
    //Decode
    always@(*)begin
        opcode = inst_in[6:0];  // opcode
        fun7 = inst_in[31:25]; // fun7
        fun3 = inst_in[14:12]; // fun3
        rs1 = inst_in[19:15];  // rs1
        rs2 = inst_in[24:20];  // rs2
        rd = inst_in[11:7];  // rd
        iimm=inst_in[31:20];//addi 指令立即数，lw指令立即数
        simm={inst_in[31:25],inst_in[11:7]}; //sw指令立即数 
        jimm={inst_in[31],inst_in[19:12],inst_in[20],inst_in[30:21]};  //jal指令立即数
        bimm={inst_in[31],inst_in[7],inst_in[30:25],inst_in[11:8]};   //beq指令立即数
    end
    
    wire ALUSrc;
    wire[4:0] ALUOp;
    wire[31:0]RD1;
    wire[31:0]RD2;
    reg signed[31:0]WD;
    wire RegWrite;
    wire [2:0] DMType;
    wire MemWrite;
    wire[31:0] 	immout;

    //生成控制信号
    Control u_ctrl(
        .Op(opcode),  //opcode
        .fun7(fun7),  //fun7 
        .fun3(fun3),    // fun3 
        .RegWrite(RegWrite), // control signal for register write
        .MemWrite(MemWrite), // control signal for memory write
        .EXTOp(EXTOp),    // control signal to signed extension
        .ALUOp(ALUOp),    // ALU opertion
        .branch(branch),    // next pc operation
        .ALUSrc(ALUSrc),   // ALU source for b
        .DMType(DMType), //dm r/w type
        .MemtoReg(MemtoReg)    // (register) write data selection  (MemtoReg)
    );
    //符号拓展
    Imm_Gen u_ext(
        .iimm(iimm),  //instr[31:20], 12 bits
        .simm(simm), //instr[31:25, 11:7], 12 bits
        .jimm(jimm), //20 bits
        .bimm(bimm), //12 bits
        .EXTOp(EXTOp),
        .immout(immout)
    );
    
    reg[31:0]reg_data;
    reg[4:0] reg_addr;
    
     //寄存器每周期顺序取数
     //循环展示前10个寄存器的值
    always@(posedge Clk_CPU or negedge rstn)begin
        if (!rstn) begin reg_addr=5'b0;end 
        else if(sw_i[13] == 1'b1)begin
            if(reg_addr < 9)begin 
                reg_data={{3'b0},reg_addr[4:0],u_rf.rf[reg_addr][23:0]};
                reg_addr=reg_addr + 1;end
            else begin
                reg_addr = 5'b00000;
            end
        end 
    end
    
    //寄存器修改和取数
    RF u_rf(
        .clk(Clk_CPU),
        .rstn(rstn),
        .RFWr(RegWrite),
        .sw_i(sw_i[15:0]),
        .A1(rs1),
        .A2(rs2),
        .A3(rd),
        .WD(WD),
        .RD1(RD1),
        .RD2(RD2)
    );
    //alu
    wire signed [31:0] A;
    wire signed[31:0] B ;
    wire Zero;
    wire signed[31:0] aluout;

    //alu_mux
    assign A = (branch == 2'b01)? rom_addr:RD1;
    //assign A = RD1;
    assign B = (ALUSrc == 1'b1)?immout:RD2;
    //alu计算地址和算术式
    ALU u_alu(
        .A(A),
        .B(B),
        .ALUOp(ALUOp),
        .C(aluout),
        .Zero(Zero)
    );

    
    reg [4:0] dmem_addr;
    reg [31:0]dmem_data;
    reg [31:0]dm_din;
    reg [5:0]dm_addr;
    wire[31:0] dm_out;
    
    //循环显示前16个dm内容
    always @(posedge Clk_CPU or negedge rstn) begin
        if(!rstn) begin  
            dmem_addr = 5'b00000;
            dmem_data = 32'hFFFFFFFF; end
        else if(sw_i[11] == 1'b1)begin
            dmem_addr = dmem_addr + 1'b1;
            dmem_data = {dmem_addr[3:0],{20'b0},u_dm.dmem[dmem_addr][7:0]};
            if(dmem_addr >= 5'b10000 )begin
                dmem_addr = 5'b00000;
                dmem_data = 32'hFFFFFFFF;end
        end
    end
    //修改数据存储
    DM u_dm(
        .clk(Clk_CPU),
        .DMWr(MemWrite),
        .addr(aluout),
        .din(RD2),
        .DMType(DMType),
        .dout(dm_out)
    );
    
    //wd_mux
    always @(*) begin
	   case(MemtoReg)
		  `MemtoReg_FromALU: WD <= aluout;
		  `MemtoReg_FromMEM: WD <= dm_out;
		  `MemtoReg_FromPC: WD  <= rom_addr + 1;
	   endcase
    end
    // 加法器
    reg signed[31:0] temp;
    always @(*) begin 
        temp = immout;
    end
    wire signed[31:0] offset = rom_addr + (temp >>> 2);   
    // 根据立即数与原有PC值相加的结果修改PC地址
    // rom每周期地址顺序取数
    always @(posedge Clk_CPU or negedge rstn) begin
        if(!rstn) begin rom_addr = 5'b0; end
        else begin
            if (rom_addr < 7 & rom_addr >=0) begin
                if(sw_i[1]==1'b0 & branch==2'b00)begin
                    rom_addr = rom_addr + 1'b1;
                end else if(branch == 2'b01) begin 
                        // 是jal指令
                        rom_addr = aluout;  
                    end
                    else if(branch == 2'b10) begin
                        // 是beq指令
                        if(Zero == 1'b1) begin 
                            rom_addr = offset;
                        end
                        else rom_addr = rom_addr + 1'b1; 
                    end
                    else if(branch == 2'b11) begin
                        // 是jalr指令
                        rom_addr = aluout;
                    end
            end
            else
                rom_addr = 5'b00000; 
            end
        end
    reg[31:0] display_data;
    //显示模块数据选择
    always @(sw_i) begin
        if(sw_i[0] == 0)begin
            case (sw_i[14:11])
                4'b1000 : display_data = inst_in;
                4'b0100 : display_data = reg_data;
                4'b0010 : display_data = aluout;
                4'b0001 : display_data = dmem_data;
            default: display_data = inst_in;
            endcase
        end
    end
    //显示模块例化
    seg7x16 u_seg7x16(
        .clk(clk),
        .rstn(rstn),
        .i_data(display_data),
        .disp_mode(sw_i[0]),
        .o_seg(disp_seg_o),
        .o_sel(disp_an_o)
    );
endmodule