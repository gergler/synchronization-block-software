`timescale 1ns/10ps

module fsm_self_test_tb();
	
    logic clock = 0;
	logic [7:0] state;
	
	localparam CLOCK = 2.5;
	
    always clock = #CLOCK ~clock;
	
    fsm_self_test fsm_self_tst(
        .clock(clock), 
        .state_out(state)
    );

    
endmodule
