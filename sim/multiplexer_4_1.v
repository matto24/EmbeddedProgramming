//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2023.2 (lin64) Build 4029153 Fri Oct 13 20:13:54 MDT 2023
//Date        : Wed Feb 21 15:44:48 2024
//Host        : madsxps running 64-bit Ubuntu 22.04.3 LTS
//Command     : generate_target multiplexer_4_1.bd
//Design      : multiplexer_4_1
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "multiplexer_4_1,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=multiplexer_4_1,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=14,numReposBlks=14,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=14,numPkgbdBlks=0,bdsource=USER,synth_mode=Hierarchical}" *) (* HW_HANDOFF = "multiplexer_4_1.hwdef" *) 
module multiplexer_4_1
   (i0,
    i1,
    i2,
    i3,
    o,
    s0,
    s1);
  input i0;
  input i1;
  input i2;
  input i3;
  output o;
  input s0;
  input s1;

  wire OR_gate_0_o;
  wire OR_gate_1_o;
  wire OR_gate_2_o;
  wire and_gate_0_o;
  wire and_gate_1_o;
  wire and_gate_2_o;
  wire and_gate_3_o;
  wire and_gate_4_o;
  wire and_gate_5_o;
  wire and_gate_6_o;
  wire and_gate_7_o;
  wire i0_1;
  wire i1_1;
  wire i2_1;
  wire i3_1;
  wire not_gate_0_C;
  wire not_gate_1_C;
  wire not_gate_2_C;
  wire s0_1;
  wire s1_1;

  assign i0_1 = i0;
  assign i1_1 = i1;
  assign i2_1 = i2;
  assign i3_1 = i3;
  assign o = OR_gate_2_o;
  assign s0_1 = s0;
  assign s1_1 = s1;
  multiplexer_4_1_OR_gate_0_1 OR_gate_0
       (.a(and_gate_1_o),
        .b(and_gate_2_o),
        .o(OR_gate_0_o));
  multiplexer_4_1_OR_gate_0_2 OR_gate_1
       (.a(and_gate_5_o),
        .b(and_gate_7_o),
        .o(OR_gate_1_o));
  multiplexer_4_1_OR_gate_0_3 OR_gate_2
       (.a(OR_gate_0_o),
        .b(OR_gate_1_o),
        .o(OR_gate_2_o));
  multiplexer_4_1_and_gate_0_0 and_gate_0
       (.a(s0_1),
        .b(s1_1),
        .o(and_gate_0_o));
  multiplexer_4_1_and_gate_0_1 and_gate_1
       (.a(not_gate_0_C),
        .b(i0_1),
        .o(and_gate_1_o));
  multiplexer_4_1_and_gate_0_2 and_gate_2
       (.a(and_gate_3_o),
        .b(i1_1),
        .o(and_gate_2_o));
  multiplexer_4_1_and_gate_0_3 and_gate_3
       (.a(not_gate_1_C),
        .b(s1_1),
        .o(and_gate_3_o));
  multiplexer_4_1_and_gate_3_0 and_gate_4
       (.a(s0_1),
        .b(not_gate_2_C),
        .o(and_gate_4_o));
  multiplexer_4_1_and_gate_3_1 and_gate_5
       (.a(and_gate_4_o),
        .b(i2_1),
        .o(and_gate_5_o));
  multiplexer_4_1_and_gate_3_2 and_gate_6
       (.a(s0_1),
        .b(s1_1),
        .o(and_gate_6_o));
  multiplexer_4_1_and_gate_3_3 and_gate_7
       (.a(and_gate_6_o),
        .b(i3_1),
        .o(and_gate_7_o));
  multiplexer_4_1_not_gate_0_2 not_gate_0
       (.A(and_gate_0_o),
        .C(not_gate_0_C));
  multiplexer_4_1_not_gate_0_3 not_gate_1
       (.A(s0_1),
        .C(not_gate_1_C));
  multiplexer_4_1_not_gate_1_0 not_gate_2
       (.A(s1_1),
        .C(not_gate_2_C));
endmodule
