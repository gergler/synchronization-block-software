`timescale 1ns/10ps

import types_pkg::*;

module mux_tb;
    
	logic clock;
    logic reset = 0;
    int   strategy_sel;// = 1;

    input_signals_t  in;
    output_signals_t out;
    parameters_t     par = '0;
	
    /* from sim_models.sv */
	clock_model _clk0 (.clock(clock));
    data_generator _gen(.data(in));
    
	//inv_strategy _dut(.*);
    strategy_mux _dut(.*, .strategy_sel(strategy_sel[7:0]));
    
    initial begin
        strategy_sel = 0;
        #1000;
        strategy_sel = 1;
        #1000;
        strategy_sel = 2;
        #1000;
        strategy_sel = 3;
        #1000;
        strategy_sel = 4;
        #1000;
    end

endmodule
