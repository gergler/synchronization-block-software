`timescale 1ns/10ps

import types_pkg::*;

module fsm_experiment_tb();

	logic clock = 0;
    logic reset = 0;
    logic start = 0;
    logic fg_opto = 0;
    logic fg_open = 0;
	logic wire_sensor = 0;
    logic detector_ready = 1;
	
    logic detonator_triggered = 0;
    logic output_trigger = 0;
    logic [7:0] scenario_state;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 10 * 1000_000; // 10ms
    localparam FG_OPEN_DELAY = 400_000;    // 100us
	localparam DETECTOR_PROLONG = 6400us;
	localparam DETECTOR_READY_TIMEOUT = 7_000_000/5; //5*100; 
	localparam DETONATE_LEN = 1000/5;
	localparam TRIGGER_LEN = 1000/5;

    always clock = #CLOCK ~clock;
	
	reset_generate reset_gen(.*);
	start_generate start_gen(.*); 
	fg_opto_generate fg_opto_gen(.*, .fg_period(FG_PERIOD), .fg_opened(FG_OPEN_DELAY));
	fg_open_generate fg_open_gen(.*, .fg_opened(FG_OPEN_DELAY));
	wire_sensor_generate wire_sensor_gen(.*);
	detector_ready_generate detector_ready_gen(.*, .detector_prolong(DETECTOR_PROLONG));
	
	input_signals_t  in = '{start, fg_opto, 0, wire_sensor, detector_ready};
	output_signals_t out = '{detonator_triggered, output_trigger, [7:0] scenario_state};
    parameters_t     par = '{FG_OPEN_DELAY, DETECTOR_READY_TIMEOUT, 0, DETONATE_LEN, TRIGGER_LEN};
    
    fsm_experiment fsm_exp(
        .clock(clock), 
		.reset_signal(reset),
        .in(in), 
        .out(out),
		.par(par)
    );
    
endmodule
