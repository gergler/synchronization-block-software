import types_pkg::*;


/* no-operation, pass value w/o modification */
module nop_strategy (
    input  clock, reset,
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);

always_comb
	out <= in;
	
endmodule


module xor_strategy (
    input  clock, reset,
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);

always_comb
	out <= in ^ par;
	
endmodule


module inv_strategy (
    input  clock, reset,
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);

always_comb
	out <= ~in;
	
endmodule
