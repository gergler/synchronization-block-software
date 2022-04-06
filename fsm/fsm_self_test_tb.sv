`timescale 1ns/10ps

module fsm_self_test_tb();
	
    logic clock = 0;
    logic fg_opto = 0;
    logic fg_open = 0;

	int fg_delay = 9_000_000/5;
    int counter = 0;
    
    localparam CLOCK = 2.5;
    localparam FG_PERIOD = 20 * 1000_000; // 10ms
    localparam FG_OPENED = 100_000;    // 100us
	
    always clock = #CLOCK ~clock;
	fg_opto_generate fg_opto_gen(.*, .fg_period(FG_PERIOD), .fg_opened(FG_OPENED));
	fg_open_generate fg_open_gen(.*, .fg_opened(FG_OPENED));
	
    
    fsm_self_test fsm_self_tst(
        .clock(clock), 
        .fg_signal(fg_opto), 
		.fg_delay(fg_delay),
		
        .counter_out(counter)
    );

    
endmodule