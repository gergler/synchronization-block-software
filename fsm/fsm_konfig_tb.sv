`timescale 1ns/10ps

module fsm_konfig_tb();

    logic clock = 0;
	logic reset = 0;
    logic start_condition =0;
    logic fast_gate = 0;
    logic fast_gate_open = 0;
    logic detector_ready = 1;
    logic output_trigger = 0;
	
	logic [2:0] scenario_state;
    int counter = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 10 * 1000_000; // 10ms
    localparam FG_OPENED = 100_000;    // 100us
    localparam BOUNCE = 10;
    
    always clock = #CLOCK ~clock;
	 
	 initial begin 
		reset = 1;
		#100;
		reset = 0;
	 end 
     
    //initial forever
    initial begin
        #( $urandom() % FG_PERIOD );
        repeat(10) begin
            fast_gate = 0;
            #(FG_PERIOD - FG_OPENED);
            fast_gate = 1;
            #FG_OPENED;
        end
    end
    
    always @(posedge fast_gate) begin
        #2ms;
        fast_gate_open = '1;
        #FG_OPENED;
        fast_gate_open = '0;
    end
    
    initial begin
        start_condition = 0;
        #10ms;
        start_condition = 1;
        #30ms;
        start_condition = 0;
    end
    
    always @(posedge output_trigger) begin
        if (reset == '0) begin
            detector_ready = 0;
            #6400us;
            detector_ready = 1;
        end;
    end
    
    fsm_test fsm_tst(
        .clock(clock), 
		.reset(reset),
        .start_signal(start_condition), 
        .fg_signal(fast_gate), 
		
        .output_trigger(output_trigger),
		.scenario_state(scenario_state),
        .counter_(counter)
    );

    
endmodule
