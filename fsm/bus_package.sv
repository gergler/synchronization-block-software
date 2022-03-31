`ifndef GUARD_BUS_PACKAGE
`define GUARD_BUS_PACKAGE

//package bus_package;

interface input_ports_bus();
	logic reset_signal; 
	logic start_signal;
	logic fg_signal;
	logic phase_signal;
	logic wire_signal;
	logic detector_ready;
endinterface

interface parameters_ports_bus();
	logic fg_open_delay;  
	logic detectr_ready_timeout;
	logic phase_shift;
	logic detonate_len;
	logic trigger_len;
endinterface

interface output_ports_bus();
	logic detonation_signal; 
	logic output_trigger;
	logic [7:0] scenario_state; 
	logic counter_out;
endinterface

//endpackage
`endif
