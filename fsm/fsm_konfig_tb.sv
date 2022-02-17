`timescale 1ns/10ps

module fsm_konfig_tb();

    logic clock = 0;
	 logic reset = 0;
    logic start_condition =0;
    logic fast_gate = 0;
    logic fast_gate_open = 0;
    logic detector_ready = 1;
    logic detonator_triggered = 0;
    logic wire_sensor = 0;
    logic detector_trigger = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 10 * 1000_000; // 10ms
    localparam FG_OPENED = 100_000;    // 100us
    localparam BOUNCE = 10;
    
    always clock = #CLOCK ~clock;
	 
	 initial begin 
		#2ms;
		reset = 1;
		#100ms;
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
        #25ms;
        start_condition = 1;
        #100us;
        start_condition = 0;
    end
    
    always @(posedge detector_trigger) begin
        detector_ready = 0;
        #6400us;
        detector_ready = 1;
    end
    
    fsm_test fsm_tst(
        .clock(clock), 
		  .reset(reset),
        .start_signal(start_condition), 
        .fg_signal(fast_gate), 
        .detector_trigger(detector_trigger)
    );

    
endmodule
