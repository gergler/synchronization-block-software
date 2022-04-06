`timescale 1ns/10ps

module fsm_calibration_tb();
	
    logic clock = 0;
	logic phase = 0;
	
	logic reset = 0;
    logic start = 0;
    logic fg_opto = 0;
    logic fg_open = 0;
    logic detector_ready = 1;
	
    logic output_trigger = 0; 
	logic [7:0] scenario_state;
    int counter = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 20 * 1000_000; // 10ms
    localparam FG_OPENED = 100_000;    // 100us
    localparam PHASE_HALF_PERIOD = 600; 
	localparam DETECTOR_PROLONG = 6400us;
	
    
    always clock = #CLOCK ~clock;
    always phase = #PHASE_HALF_PERIOD ~phase;
	 
	reset_generate reset_gen(.*);
	start_generate start_gen(.*);
	fg_opto_generate fg_opto_gen(.*, .fg_period(FG_PERIOD), .fg_opened(FG_OPENED));
	fg_open_generate fg_open_gen(.*, .fg_opened(FG_OPENED));
	detector_ready_generate detector_ready_gen(.*, .detector_prolong(DETECTOR_PROLONG));
    
    fsm_calibration fsm_clbr(
        .clock(clock), 
		.reset_signal(reset),
        .start_signal(start), 
        .fg_signal(fg_opto), 
        .phase_signal(phase), 
        
        .output_trigger(output_trigger),
		.scenario_state(scenario_state),
        .counter_out(counter)
    );

    
endmodule
