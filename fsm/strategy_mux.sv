import types_pkg::*;

module strategy_mux (
    input  clock, reset,
    //input  [7:0] strategy_sel,
    input variants_t strategy_sel,
    
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);

/*
enum {SELF_TEST, EXPERIMENT, CALIBRATION, EXPERIMENT_PHASE, CALIBRATION_PHASE} variants; // probably, should be in types_pkg.sv
*/
output_signals_t out_n[strategy_sel.num()];

//enum output_signals_t {SELF_TEST, EXPERIMENT, CALIBRATION, EXPERIMENT_PHASE, CALIBRATION_PHASE} out_n;

fsm_experiment fsm_exp(.*, .out(out_n[EXPERIMENT]));
fsm_calibration fsm_clbr(.*, .out(out_n[CALIBRATION]));
fsm_experiment_phase fsm_exp_phase(.*, .out(out_n[EXPERIMENT_PHASE]));
fsm_calibration_phase fsm_clbr_phase(.*, .out(out_n[CALIBRATION_PHASE]));
fsm_self_test fsm_self_tst(.*, .out(out_n[SELF_TEST]));


//always_comb begin
//    //out <= out_n[strategy_sel];
//    out <= 'Z;
//    for (byte i = 0; i < strategy_sel.num(); i++)
//        if (strategy_sel == variants_t'(i))
//            out <= out_n[i];
//end

always_comb begin 
    case (strategy_sel)
        SELF_TEST:          out <= out_n[SELF_TEST];
        EXPERIMENT:         out <= out_n[EXPERIMENT];
        CALIBRATION:        out <= out_n[CALIBRATION];
        EXPERIMENT_PHASE:   out <= out_n[EXPERIMENT_PHASE];
        CALIBRATION_PHASE:  out <= out_n[CALIBRATION_PHASE];
        default:            out <= '0;
    endcase
end

endmodule
