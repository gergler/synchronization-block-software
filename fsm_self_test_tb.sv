`timescale 1ns/10ps

import types_pkg::*;

module fsm_self_test_tb();
	
    logic clock = 0;
    logic reset = 0;
	
	localparam CLOCK_PERIOD = 5;
    clock_module #(CLOCK_PERIOD) clock_sim_model(.clock(clock));
    
    input_signals_t  in;
    output_signals_t out;
    parameters_t     par;
    
    reset_module           reset_sim_model(.reset(reset));
	start_module           start_sim_model(.start(in.start)); 
	
    fsm_self_test fsm_self_tst(
        .clock(clock), 
        .reset(reset),
        .in(in), 
        .out(out)
    );

    
endmodule
