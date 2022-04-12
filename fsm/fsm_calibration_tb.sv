`timescale 1ns/10ps

import types_pkg::*;

module fsm_calibration_tb();
	
    logic clock = 0;
	logic reset = 0;
    logic fg_open = 0;
    
    input_signals_t  in;
    output_signals_t out;
    parameters_t     par;
    
    localparam CLOCK_PERIOD = 5;
      
    assign par.FG_OPEN_DELAY          = 5_000_000/CLOCK_PERIOD;
    assign par.DETECTOR_READY_TIMEOUT = 7_000_000/CLOCK_PERIOD;
    assign par.PHASE_SHIFT            =       700/CLOCK_PERIOD; 
    assign par.DETONATE_LEN           =      1000/CLOCK_PERIOD;
    assign par.TRIGGER_LEN            =      1000/CLOCK_PERIOD;
    
    clock_module #(CLOCK_PERIOD)         clk_sim_model (.clock(clock));
	
	reset_module              reset_sim_model(.reset(reset));
	start_module              start_sim_model(.start(in.start)); 
	fg_opto_module            fg_opto_sim_model(.fg_opto(in.fg_opto));
	fg_open_module            fg_open_sim_model(.reset(reset), .fg_opto(in.fg_opto), .fg_open(fg_open));
	detector_ready_module     detector_sim_model(.reset(reset), .output_trigger(out.output_trigger), .detector_ready(in.detector_ready)); 
    
    fsm_calibration fsm_clbr(
        .clock(clock), 
		.reset(reset),
        .in(in), 
        .out(out),
		.par(par)
    );

    
endmodule
