`timescale 1ns/10ps

module fsm_calibration_tb();

    logic clock = 0;
	logic reset = 0;
    logic start_condition =0;
    logic fast_gate_opto = 0;
    logic fast_gate_open = 0;
    logic detector_ready = 1;
    logic detonator_triggered = 0;
    logic output_trigger = 0;
    logic phase_signal = 0; 
	logic [2:0] scenario_state;
    int counter = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 20 * 1000_000; // 10ms
    localparam FG_OPENED = 100_000;    // 100us
    localparam PHASE_HALF_PERIOD = 600; 
    
    always clock = #CLOCK ~clock;
    
    always phase_signal = #PHASE_HALF_PERIOD ~phase_signal;
	 
	 initial begin 
		reset = 1;
		#100;
		reset = 0;
	 end 
     
    //initial forever
    initial begin
        #( $urandom() % FG_PERIOD );
        repeat(10) begin
            fast_gate_opto = 0;
            #(FG_PERIOD - FG_OPENED);
            fast_gate_opto = 1;
            #FG_OPENED;
        end
    end
    
    always @(posedge fast_gate_opto) begin
        #9ms;
        fast_gate_open = '1;
        #FG_OPENED;
        fast_gate_open = '0;
    end
    
    initial begin
        start_condition = 0;
        #10ms;
        start_condition = 1;
        #20ms;
        start_condition = 0;
        #10ms;
        start_condition = 1;
        #20ms;
        start_condition = 0;
    end
    
    always @(posedge output_trigger) begin
        if (reset == '0) begin
            detector_ready = 0;
            #6400us;
            detector_ready = 1;
        end;
    end
    
    fsm_calibration fsm_clbr(
        .clock(clock), 
		.reset_signal(reset),
        .start_signal(start_condition), 
        .fg_signal(fast_gate_opto), 
        .phase_signal(phase_signal), 
        //.phase_shift(phase_shift),
        
        .output_trigger(output_trigger),
		.scenario_state(scenario_state),
        .counter_out(counter)
    );

    
endmodule
