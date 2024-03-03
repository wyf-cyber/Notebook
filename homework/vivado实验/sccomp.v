`timescale 1ns / 1ps
// sccomp原版存档（未删改）


module sccomp(
    input clk,
    input rstn,
    input [15:0]sw_i,
    output [7:0]disp_an_o,
    output [7:0]disp_seg_o
    );
       //产生最终的编码
    wire[31:0] reg_data;
    reg[31:0] alu_disp_data;
    wire[31:0] dmem_data;
    //计数分频
    reg[31:0]clkdiv;
    wire Clk_CPU;
    always@(posedge clk or negedge rstn) begin
        if(!rstn) clkdiv<=0;
        else clkdiv<=clkdiv+1'b1; end

    //根据开关控制快慢
    assign Clk_CPU=(sw_i[15])? clkdiv[27] : clkdiv[25];

   //display_data为最终的编码
    reg[63:0] display_data;
    //led_data_addr用来循环
    reg[5:0]led_data_addr;
    //led_disp_data为产生的暂时的编码
    reg[63:0]led_disp_data;
    parameter LED_DATA_NUM=48;

    //共19个64位的编码，对应图形显示
    reg[63:0]LED_DATA[47:0];
    initial begin
//        LED_DATA[0]=64'hC6F6F6F0C6F6F6F0;
//        LED_DATA[1]=64'hF9F6F6CFF9F6F6CF;
//        LED_DATA[2]=64'hFFC6F0FFFFC6F0FF;
//        LED_DATA[3]=64'hFFC0FFFFFFC0FFFF;
//        LED_DATA[4]=64'hFFA3FFFFFFA3FFFF;
//        LED_DATA[5]=64'hFFFFA3FFFFFFA3FF;
//        LED_DATA[6]=64'hFFFF9CFFFFFF9CFF;
//        LED_DATA[7]=64'hFF9EBCFFFF9EBCFF;
//        LED_DATA[8]=64'hFF9CFFFFFF9CFFFF;
//        LED_DATA[9]=64'hFFC0FFFFFFC0FFFF;
//        LED_DATA[10]=64'hFFA3FFFFFFA3FFFF;
//        LED_DATA[11]=64'hFFA7B3FFFFA7B3FF;
//        LED_DATA[12]=64'hFFC6F0FFFFC6F0FF;
//        LED_DATA[13]=64'hF9F6F6CFF9F6F6CF;
//        LED_DATA[14]=64'h9EBEBEBC9EBEBEBC;
//        LED_DATA[15]=64'h2737373327373733;
//        LED_DATA[16]=64'h505454EC505454EC;
//        LED_DATA[17]=64'h744454F8744454F8;
//        LED_DATA[18]=64'h0062080000620800;

        LED_DATA[0]=64'hFFFFFFFEFEFEFEFE;
        LED_DATA[1]=64'hFFFEFEFEFEFEFFFF;
        LED_DATA[2]=64'hDEFEFEFEFFFFFFFF;
        LED_DATA[3]=64'hCEFEFEFFFFFFFFFF;
        LED_DATA[4]=64'hC2FFFFFFFFFFFFFF;
        LED_DATA[5]=64'hC1FEFFFFFFFFFFFF;
        LED_DATA[6]=64'hF1FCFFFFFFFFFFFF;
        LED_DATA[7]=64'hFDF8F7FFFFFFFFFF;
        LED_DATA[8]=64'hFFF8F3FFFFFFFFFF;
        LED_DATA[9]=64'hFFF8F1FEFFFFFFFF;
        LED_DATA[10]=64'hFFFFF9F8FFFFFFFF;
        LED_DATA[11]=64'hFFFFFDF8F7FFFFFF;
        LED_DATA[12]=64'hFFFFFFF9F1FFFFFF;
        LED_DATA[13]=64'hFFFFFFFFF1FCFFFF;
        LED_DATA[14]=64'hFFFFFFFFF9F8FFFF;
        LED_DATA[15]=64'hFFFFFFFFFFF8F3FF;
        LED_DATA[16]=64'hFFFFFFFFFFFBF1FE;
        LED_DATA[17]=64'hFFFFFFFFFFFFF9BC;
        LED_DATA[18]=64'hFFFFFFFFFFFFBDBC;
        LED_DATA[19]=64'hFFFFFFFFBFBFBFBD;
        LED_DATA[20]=64'hFFFFBFBFBFBFFFFF;
        LED_DATA[21]=64'hFFBFBFBFBFBFFFFF;
        LED_DATA[22]=64'hAFBFBFBFFFFFFFFF;
        LED_DATA[23]=64'h2737FFFFFFFFFFFF;
        LED_DATA[24]=64'h277777FFFFFFFFFF;
        LED_DATA[25]=64'h7777777777FFFFFF;
        LED_DATA[26]=64'hFFFF7777777777FF;
        LED_DATA[27]=64'hFFFFFF7777777777;
        LED_DATA[28]=64'hFFFFFFFFFF777771;
        LED_DATA[29]=64'hFFFFFFFFFFFF7750;
        LED_DATA[30]=64'hFFFFFFFFFFFFFFC8;
        LED_DATA[31]=64'hFFFFFFFFFFFFE7CE;
        LED_DATA[32]=64'hFFFFFFFFFFFFC7CF;
        LED_DATA[33]=64'hFFFFFFFFFFDEC7FF;
        LED_DATA[34]=64'hFFFFFFFFF7CEDFFF;
        LED_DATA[35]=64'hFFFFFFFFC7CFFFFF;
        LED_DATA[36]=64'hFFFFFFFEC7EFFFFF;
        LED_DATA[37]=64'hFFFFFFCECFFFFFFF;
        LED_DATA[38]=64'hFFFFE7CEFFFFFFFF;
        LED_DATA[39]=64'hFFFFC7CFFFFFFFFF;
        LED_DATA[40]=64'hFFDEC7FFFFFFFFFF;
        LED_DATA[41]=64'hF7CEDFFFFFFFFFFF;
        LED_DATA[42]=64'hA7CFFFFFFFFFFFFF;
        LED_DATA[43]=64'hA7AFFFFFFFFFFFFF;
        LED_DATA[44]=64'hAFBFBFBFFFFFFFFF;
        LED_DATA[45]=64'hBFBFBFBFBFFFFFFF;
        LED_DATA[46]=64'hFFFFBFBFBFBFBFFF;
        LED_DATA[47]=64'hFFFFFFFFBFBFBFBD;
        
    end
    
always @(posedge Clk_CPU or negedge rstn) begin
        if (!rstn) begin
            led_data_addr <= 6'd0;
            led_disp_data <= 64'b1;
        end
        else if (sw_i[0]) begin
            if (led_data_addr == LED_DATA_NUM) begin
                led_data_addr <= 6'd0;
                led_disp_data <= 64'b1;
            end
            else begin
                led_disp_data <= LED_DATA[led_data_addr];
                led_data_addr <= led_data_addr + 1'b1;
            end
        end
    end

    // Instruction memory
    wire [31:0] instr;
    reg [4:0] rom_addr;
    dist_mem_gen_0 dist_mem_gen_inst (
        .a(rom_addr),
        .spo(instr)
    );

    // Register file
    reg [4:0] rf_addr;
    wire [31:0] rd1, rd2;
    reg [4:0] rs1 = 5'b0;
    reg [4:0] rs2 = 5'b0;
    reg [31:0] wd = 32'b0;
    always @(*) begin
        rs1 <= {{2{1'b0}},sw_i[10:8]};
        if(sw_i[2])wd <= {{29{sw_i[7]}},sw_i[7:5]};  // 写入32位有符号数
        else wd <= 32'b0; 
        rs2 <= {{2{1'b0}},sw_i[7:5]}; // 读取模式，输入寄存器号
    end
    RF rf_u (
        .clk(Clk_CPU),
        .rst(rstn),
        .RFWr(sw_i[2]),
        .sw_i(sw_i),
        //.A1(rf_addr),
        .A1(rs1),
        .A2(rs2),
        .A3(rs1),
        .WD(wd),
        //.RD1(reg_data),
        .RD1(rd1),
        .RD2(rd2)
    );
 
    
    // ALU
    // 符号扩展
    reg [4:0] alu_addr;
    reg [31:0] a = 32'b0;
    reg [31:0] b = 32'b0;
    wire signed [31:0] c;
    wire [7:0] zero;
    always@(*) begin
        a = {{28{sw_i[10]}},sw_i[10:7]};
        b = {{28{sw_i[6]}},sw_i[6:3]};
    end
    // 例化alu
    alu alu_u (
        .A(rd1),
        .B(rd2),
        .ALUOp(sw_i[4:3]),
        .C(c),
        .Zero(zero)
    );
    // Data Memory
    reg [5:0] dm_addr;
    reg [31:0] dataIn;
    reg [2:0] DMType;
    wire [31:0] DMOut;
    always@(*) begin
        dm_addr = {{3{1'b0}},sw_i[10:8]};
        dataIn = {{29{sw_i[7]}},sw_i[7:5]};
        DMType = {1'b0,sw_i[4:3]};
    end
    DM dm_u(
        .clk(Clk_CPU),   //100MHZ CLK
        .DMWr(sw_i[2]),  // write signal
        .addr(dm_addr),  // 操作地址
        .din(dataIn),   // 写入的数据
        .DMType(DMType),// 操作数据的类型 
        .dout(dmem_data)   // 输出的数据，读取结果
    );
    
   // 循环更新 ROM 和 RF 和 ALU 地址 
    parameter ROM_NUM=12;
    parameter RF_NUM=32;
    parameter ALU_NUM=5;
    always @(posedge Clk_CPU or negedge rstn) begin
        if (!rstn) begin
            rom_addr <= 5'b0;
            rf_addr <= 5'b0;
            alu_addr <= 5'b0;
        end
        else begin
            if (sw_i[14]) begin
                rom_addr <= rom_addr + 1;
                if (rom_addr == ROM_NUM) rom_addr <= 5'b0;
            end
            else if (sw_i[13]) begin
                rf_addr <= rf_addr + 1;
                if (rf_addr == RF_NUM) rf_addr <= 5'b0;
            end
            else if (sw_i[12]) begin
                alu_addr <= alu_addr + 1;
                if (alu_addr == ALU_NUM) alu_addr <= 5'b0;
            end
        end
        // 循环输出 A、B、C、Zero的值，结尾加FFFFFFFF
        case(alu_addr)
            5'b00000: alu_disp_data = rd1;
            5'b00001: alu_disp_data = rd2;
            5'b00010: alu_disp_data = c;
            5'b00011: alu_disp_data = zero;
            5'b00100: alu_disp_data = 32'hFFFFFFFF;
            default:  alu_disp_data = 32'hFFFFFFFF;
        endcase
    end
    
    // 选择输出模式
    always @* begin
        case (sw_i[14:11])
            4'b1000: display_data = instr;        // ROM content
            4'b0100: display_data = reg_data;     // RF content
            4'b0010: display_data = alu_disp_data;
            4'b0001: display_data = dmem_data;
            default: display_data = instr;
        endcase

        // sccomp mode
        if (sw_i[0] == 1'b1) 
            display_data = led_disp_data;
    end

    // Seg7x16 module instantiation
    seg7x16 u_seg7x16 (
        .clk(clk),
        .rstn(rstn),
        .i_data(display_data),
        .disp_mode(sw_i[0]),
        .o_seg(disp_seg_o),
        .o_sel(disp_an_o)
    );

endmodule
