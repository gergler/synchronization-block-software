package types_pkg;

typedef struct packed {logic start, fg_opto, phase, wire_sensor, detector_ready;} input_signals_t;
typedef struct packed {logic detonator_triggered, output_trigger, valid_scenario; logic [7:0] scenario_state;} output_signals_t;
typedef struct packed {integer FG_OPEN_DELAY, DETECTOR_READY_TIMEOUT, PHASE_SHIFT, DETONATE_LEN, TRIGGER_LEN;} parameters_t;

typedef enum byte {SELF_TEST, EXPERIMENT, CALIBRATION, EXPERIMENT_PHASE, CALIBRATION_PHASE} variants_t;

endpackage
