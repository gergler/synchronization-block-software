`timescale 1ns/10ps

import types_pkg::*;

module fsm_calibration_phase_tb();
	
    logic clock = 0;
	logic phase = 0;
	logic reset = 0;
    logic start = 0;
    logic fg_opto = 0;
    logic fg_open = 0;
    logic detector_ready = 1;
	
    logic output_trigger = 0; 
	logic [7:0] scenario_state;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 20 * 1000_000; // 10ms
    localparam PHASE_HALF_PERIOD = 600; 
	localparam DETECTOR_PROLONG = 6400us;
	  
    always clock = #CLOCK ~clock;
    always phase = #PHASE_HALF_PERIOD ~phase;
	 
	reset_generate reset_gen(.*);
	start_generate start_gen(.*);
	fg_opto_generate fg_opto_gen(.*, .fg_period(FG_PERIOD), .fg_opened(FG_OPEN_DELAY));
	fg_open_generate fg_open_gen(.*, .fg_opened(FG_OPEN_DELAY));
	detector_ready_generate detector_ready_gen(.*, .detector_prolong(DETECTOR_PROLONG));
	
	localparam FG_OPEN_DELAY = 100_000;    // 100us
	localparam PHASE_SHIFT = (700/5 - 1);
	localparam TRIGGER_LEN = 1000/5;
	
	input_signals_t  in = '{start, fg_opto, phase, 0, detector_ready};
	output_signals_t out = '{0, output_trigger, scenario_state};
    parameters_t     par = '{FG_OPEN_DELAY, 0, PHASE_SHIFT, 0, TRIGGER_LEN};
    
    fsm_calibration_phase fsm_clbr_phase(
        .clock(clock), 
		.reset_signal(reset),
        .in(in), 
        .out(out),
		.par(par)
    );

    
endmodule
