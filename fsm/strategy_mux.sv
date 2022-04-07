import types_pkg::*;


module strategy_mux (
    input  clock, reset,
    input  [7:0] strategy_sel,
    
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);


enum {EXPERIMENT, CALIBRATION, EXPERIMENT_PHASE, CALIBRATION_PHASE, SELF_TEST} variants; /* probably, should be in types_pkg.sv */

output_signals_t out_n[variants.num()];

fsm_experiment fsm_exp(.*, .out(out_n[EXPERIMENT]));
fsm_calibration fsm_clbr(.*, .out(out_n[CALIBRATION]));
fsm_experiment_phase fsm_exp_phase(.*, .out(out_n[EXPERIMENT_PHASE]));
fsm_calibration_phase fsm_clbr_phase(.*, .out(out_n[CALIBRATION_PHASE]));
fsm_self_test fsm_self_tst(.*, .out(out_n[SELF_TEST]));


always_comb begin
    out = 'Z;
    for (int i = 0; i < variants.num(); i++)
        if (strategy_sel == i)
            out = out_n[i];
end


endmodule
