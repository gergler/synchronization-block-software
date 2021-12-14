`timescale 1ns/1ps

module fsm_tb();

    logic clock = 0;
    logic start_condition =0;
    logic fast_gate = 0;
    logic detector_ready = 0;
    logic detonator_triggered = 0;
    logic wire_sensor = 0;
    logic output_trigger = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 10 * 1000_000; // 10ms
    localparam FG_OPENED = 100 * 1000;    // 100us
    localparam BOUNCE = 10;
    
//    initial forever begin  
//      clock = 0;
//      #CLOCK;
//      clock = 1;
//      #CLOCK;
//    end  

    always clock = #CLOCK ~clock;
     
    initial repeat(10) begin
        fast_gate = 0;
        #(FG_PERIOD - FG_OPENED);
        fast_gate = 1;
        #FG_OPENED;
    end
    
    initial begin
        start_condition = 0;
        #25ms;
        start_condition = 1;
        #100;
        start_condition = 0;
        #100us;
    end
    
    always @(posedge detonator_triggered) begin
        #5ms;
        wire_sensor = 1;
        for (int i = 0; i < 10; i++) begin
            #( 100*(1+($urandom() % 10)) );
            wire_sensor = ~wire_sensor;
        end
        #100;
        wire_sensor = 1;
    end
    
    always @(posedge output_trigger) begin
        #64us;
        detector_ready = 1;
        #100;
        detector_ready = 0;
    end
    
    fsm_experiment fsm_exp(
        .clock(clock), 
        .start_signal(start_condition), 
        .fg_signal(fast_gate), 
        .wire_signal(wire_sensor), 
        .detector_ready(detector_ready),
        .detonation_signal(detonator_triggered),
        .output_trigger(output_trigger)
    );
    //fsm_tst fsm_test(.clock(clock), .start_signal(start_condition), .fg_signal(fast_gate), .detector_signal(output_trigger));

    
endmodule
