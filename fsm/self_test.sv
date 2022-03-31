`timescale 1ns/10ps

module self_test();
	logic control_reg = 0;
	logic clock = 0;
    logic reset = 0;
    logic start_condition =0;
    logic fast_gate_opto = 0;
//    logic fast_gate_open = 0;
	logic phase_signal = 0;
    logic detector_ready = 1;
    logic wire_sensor = 0;
	
	localparam fg_open_delay = 0; 
	localparam detector_ready_timeout = 0; 
	localparam phase_shift = 0; 
	localparam detonate_len = 0;
	localparam trigger_len = 0;
	
	logic detonator_triggered = 0;
    logic output_trigger = 0;
    logic [7:0] scenario_state;
    int counter = 0;
	logic [2:0] scenario_reg = 0;

	localparam CLOCK = 2.5;
	
	always clock = #CLOCK ~clock;
	
	initial begin
		for (control_reg = 0; control_reg < 9; control_reg = control_reg + 1) begin
			#10us;
			if (control_reg == 4)
				control_reg = 0;
		end		
	end
	
	scenario_multiplexer scenario_mux(
		.control_reg_input(control_reg),
		.clock_reg_input(clock), 
		.reset_reg_input(reset),
		.start_reg_input(start_condition), 
		.fg_reg_input(fast_gate_opto), 
		.phase_reg_input(phase_signal), 
		.wire_reg_input(wire_sensor), 
		.detector_ready_reg_input(detector_ready),
		
		.fg_open_delay_reg_input(fg_open_delay), 
		.detectr_ready_timeout_reg_input(detector_ready_timeout), 
		.phase_shift_reg_input(phase_shift), 
		.detonate_len_reg_input(detonate_len), 
		.trigger_len_reg_input(trigger_len),
		
		.detonation_signal_reg_output(detonator_triggered),
		.trigger_reg_output(output_trigger),
		.scenario_state_reg_output(scenario_state),
		.counter_reg_output(counter),
		.scenario_reg_output(scenario_reg)
);
    
endmodule
