`timescale 1ns/10ps

import types_pkg::*;

module mux_tb;

	logic clock = 0;
    logic reset = 0;
    variants_t strategy_sel;// = 1;
    
    logic fg_open = 0;
    logic start = 0;

	input_signals_t  in;
	output_signals_t out;
    parameters_t     par;
    
    assign par.FG_OPEN_DELAY = 400_000/5;
    assign par.DETECTOR_READY_TIMEOUT = 7_000_000/5;
    assign par.PHASE_SHIFT = (700/5 - 1); 
    assign par.DETONATE_LEN = 1000/5;
    assign par.TRIGGER_LEN = 1000/5;
    
    assign in.start = start;

    logic [7:0] scenario_state;
	
    clock_module #(5)    clock_sim_model(.clock(clock));
    clock_module #(1200) phase_sim_model(.clock(in.phase));
	
	reset_module              reset_sim_model(.reset(reset));
	//start_module              start_sim_model(.start(in.start_signal)); 
	fg_opto_module            fg_opto_sim_model(.fg_opto(in.fg_opto));
	fg_open_module            fg_open_sim_model(.reset(reset), .fg_opto(in.fg_opto), .fg_open(fg_open));
	wire_sensor_module        wire_sensor_sim_model(.reset(reset), .detonator_triggered(out.detonator_triggered), .reset(reset), .wire_sensor(in.wire_sensor));
	detector_ready_module     detector_sim_model(.reset(reset), .output_trigger(out.output_trigger), .detector_ready(in.detector_ready));
	
    strategy_mux _dut(.*, .strategy_sel(strategy_sel));
    
    initial begin
        for(int i = 0; i < 5; i++) begin
            strategy_sel = variants_t'(i);
            start = 0;
            #100ns;
            start = 1;
            #100ns;
            start = 0;
            #50ms;
        end  
    end

endmodule
