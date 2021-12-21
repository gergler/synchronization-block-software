`timescale 1ns/1ps

module tb_scenario1();

    logic start_condition =0;
    logic fast_gate = 0;
    logic phase_signal = 0;
    logic detonator_triggered = 0;
    logic wire_sensor = 0;
    logic output_trigger = 0;
    
    localparam FG_PERIOD = 10 * 1000000;
    localparam FG_OPENED = 100 * 1000; 
    localparam PH_FREQUENCY = 818924; // Hz
    localparam PH_PERIOD = 1000_000_000/PH_FREQUENCY; //1200;
    localparam PH_OPENED = (PH_PERIOD / 2);
    
    localparam BOUNCE = 10;
    
    
    initial forever begin
        phase_signal = 0;
        #(PH_PERIOD - PH_OPENED);
        phase_signal = 1;
        #PH_OPENED;
    end
    
    initial repeat(10) begin
        fast_gate = 0;
        #(FG_PERIOD - FG_OPENED);
        fast_gate = 1;
        #FG_OPENED;
    end
    
    initial begin
        start_condition = 0;
        #(25*1000_000);
        start_condition = 1;
        #100;
        start_condition = 0;
        #100_000;
        
        @(posedge fast_gate);
        #10000;
        detonator_triggered = 1;
        #100;
        detonator_triggered = 0;
    end
    
    always @(posedge detonator_triggered) begin
        #(5*1000_000);
        wire_sensor = 1;
        for (int i = 0; i < 10; i++) begin
            #( 100*(1+($urandom() % 10)) );
            wire_sensor = ~wire_sensor;
        end
        #100;
        wire_sensor = 1;
        //#1000_000;
        //wire_sensor = 0;
    end
    

endmodule