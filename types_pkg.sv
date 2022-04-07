package types_pkg;

typedef struct packed {logic start_signal, fg_signal, phase_signal, wire_signal, detector_ready;} input_signals_t;
typedef struct packed {logic detonation_signal, output_trigger; logic [7:0] scenario_state;} output_signals_t;
typedef struct packed {logic fg_open_delay, detector_ready_timeout, phase_shift, detonate_len, trigger_len;} parameters_t;

endpackage
