`timescale 1ns/10ps
`include "fsm_tb_package.sv"

module fsm_experiment_tb();
	import fsm_tb_package::*;

	logic clock = 0;
    logic reset = 0;
    logic start_condition =0;
    logic fast_gate_opto = 0;
    logic fast_gate_open = 0;
	logic phase_signal = 0;
    logic detector_ready = 1;
    logic detonator_triggered = 0;
    logic wire_sensor = 0;
    logic output_trigger = 0;
    logic [7:0] scenario_state;
    int counter = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 10 * 1000_000; // 10ms
    localparam FG_OPENED = 100_000;    // 100us
    localparam BOUNCE = 10;
    localparam PHASE_HALF_PERIOD = 600; 
	localparam DETECTOR_PROLONG = 6400us;

    always clock = #CLOCK ~clock;
	
	always phase_signal = #PHASE_HALF_PERIOD ~phase_signal;
	
	initial begin
		reset_generate(reset, 100);
	end

    initial begin
        $urandom(1);
        #($urandom_range(FG_PERIOD/2, FG_PERIOD/8));
        repeat(10) fg_opto_generate(fast_gate_opto, FG_PERIOD, FG_OPENED);
    end
    
    always @(posedge fast_gate_opto) begin
		fg_open_generate(fast_gate_open, FG_OPENED);
    end
    
    initial begin
        start_generate(start_condition, 5ms, 100us);
    end
    
    always @(posedge detonator_triggered) begin
        if (reset == '0) 
			wire_sensor_generate(wire_sensor);
    end
    
    always @(posedge output_trigger) begin
        if (reset == '0) begin
			#200ns;
			detector_ready_generate(detector_ready, DETECTOR_PROLONG);
		end
    end
    
    fsm_experiment fsm_exp(
        .clock(clock), 
		.reset_signal(reset),
        .start_signal(start_condition), 
        .fg_signal(fast_gate_opto), 
		.phase_signal(phase_signal), 
        .wire_signal(wire_sensor), 
        .detector_ready(detector_ready),
        
        .detonation_signal(detonator_triggered),
        .output_trigger(output_trigger),
        .scenario_state(scenario_state),
        .counter_out(counter)
    );
    
endmodule