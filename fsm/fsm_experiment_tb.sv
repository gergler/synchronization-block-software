`timescale 1ns/10ps

module fsm_experiment_tb();

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

    always clock = #CLOCK ~clock;
	
	always phase_signal = #PHASE_HALF_PERIOD ~phase_signal;
	 
	 initial begin 
		reset = 1;
		#100;
		reset = 0;
	 end 
     
    //initial forever
    initial begin
        fast_gate_opto = 0;
        //#( $urandom() % FG_PERIOD );
        $urandom(1);
        #( $urandom_range(FG_PERIOD/2, FG_PERIOD/8) );
        repeat(10) begin
            fast_gate_opto = 1;
            #FG_OPENED;
            fast_gate_opto = 0;
            #(FG_PERIOD - FG_OPENED);
        end
    end
    
    always @(posedge fast_gate_opto) begin
        #2ms;
        fast_gate_open = '1;
        #FG_OPENED;
        fast_gate_open = '0;
    end
    
    initial begin
        start_condition = 0;
        #5ms;
        start_condition = 1;
        #100us;
        start_condition = 0;
        //#100us;
    end
    
    always @(posedge detonator_triggered) begin
        if (reset == '0) begin
            #5us;
            wire_sensor = 1;
            for (int i = 0; i < 10; i++) begin
                #( 10*(1+($urandom() % 10)) );
                wire_sensor = ~wire_sensor;
            end
            #100;
            wire_sensor = 1;
            #1ms;
            wire_sensor = 0;
        end
    end
    
    always @(posedge output_trigger) begin
        if (reset == '0) begin
            #200ns;
            detector_ready = 0;
            #6400us;
            detector_ready = 1;
            //#100;
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
