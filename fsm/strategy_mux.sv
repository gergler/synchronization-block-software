import types_pkg::*;


module strategy_mux (
    input  clock, reset,
    input  [7:0] strategy_sel,
    
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);

/*
output_signals_t nop_out;
output_signals_t xor_out;
output_signals_t inv_out;

nop_strategy _nop(.*, .out(nop_out));
xor_strategy _xor(.*, .out(xor_out));
inv_strategy _inv(.*, .out(inv_out));

always_comb
    case(strategy_sel)
        0: out <= nop_out;
        1: out <= xor_out;
        2: out <= inv_out;
        default: out <= 'Z;
    endcase
*/

enum {NOP, XOR, INV} variants; /* probably, should be in types_pkg.sv */

output_signals_t out_n[variants.num()];

nop_strategy _nop(.*, .out(out_n[NOP]));
xor_strategy _xor(.*, .out(out_n[XOR]));
inv_strategy _inv(.*, .out(out_n[INV]));

/*
always_comb
    case(strategy_sel)
        NOP: out <= out_n[NOP];
        XOR: out <= out_n[XOR];
        INV: out <= out_n[INV];
        default: out <= 'Z;
    endcase
*/

always_comb begin
    out = 'Z;
    for (int i = 0; i < variants.num(); i++)
        if (strategy_sel == i)
            out = out_n[i];
end


endmodule
