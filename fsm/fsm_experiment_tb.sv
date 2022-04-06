`timescale 1ns/10ps

module fsm_experiment_tb();

	logic clock = 0;
	logic phase = 0;

    logic reset = 0;
    logic start = 0;
    logic fg_opto = 0;
    logic fg_open = 0;
	logic wire_sensor = 0;
    logic detector_ready = 1;
	
    logic detonator_triggered = 0;
    logic output_trigger = 0;
    logic [7:0] scenario_state;
    int counter = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 10 * 1000_000; // 10ms
    localparam FG_OPENED = 100_000;    // 100us
    localparam PHASE_HALF_PERIOD = 600; 
	localparam DETECTOR_PROLONG = 6400us;

    always clock = #CLOCK ~clock;
	always phase = #PHASE_HALF_PERIOD ~phase;
	
	reset_generate reset_gen(.*);
	start_generate start_gen(.*); 
	fg_opto_generate fg_opto_gen(.*, .fg_period(FG_PERIOD), .fg_opened(FG_OPENED));
	fg_open_generate fg_open_gen(.*, .fg_opened(FG_OPENED));
	wire_sensor_generate wire_sensor_gen(.*);
	detector_ready_generate detector_ready_gen(.*, .detector_prolong(DETECTOR_PROLONG));
    
    fsm_experiment fsm_exp(
        .clock(clock), 
		.reset_signal(reset),
        .start_signal(start_condition), 
        .fg_signal(fast_gate_opto), 
		.phase_signal(phase), 
        .wire_signal(wire_sensor), 
        .detector_ready(detector_ready),
        
        .detonation_signal(detonator_triggered),
        .output_trigger(output_trigger),
        .scenario_state(scenario_state),
        .counter_out(counter)
    );
    
endmodule
