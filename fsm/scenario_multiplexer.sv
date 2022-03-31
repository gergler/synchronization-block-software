module scenario_multiplexer (
		input [2:0] control_reg_input,
		input clock_reg_input, reset_reg_input, start_reg_input, fg_reg_input, phase_reg_input, wire_reg_input, detector_ready_reg_input,
		
		input fg_open_delay_reg_input, detectr_ready_timeout_reg_input, phase_shift_reg_input, detonate_len_reg_input, trigger_len_reg_input,
		
		output detonation_signal_reg_output, trigger_reg_output, 
		output [7:0] scenario_state_reg_output, output int counter_reg_output,
		
		output [2:0] scenario_reg_output
);

reg [2:0] control_reg =0;
assign control_reg = control_reg_input;

assign scenario_reg_output = control_reg;

reg [2:0] control_reg_history = '0;

struct {
	logic clock;
	logic reset_signal; 
	logic start_signal;
	logic fg_signal;
	logic phase_signal;
	logic wire_signal;
	logic detector_ready;
} input_ports;

assign input_ports = '{clock_reg_input, reset_reg_input, start_reg_input, fg_reg_input, phase_reg_input, wire_reg_input, detector_ready_reg_input};

struct {
	logic fg_open_delay;  
	logic detectr_ready_timeout;
	logic phase_shift;
	logic detonate_len;
	logic trigger_len;
} parameters_ports;

assign parameters_ports = '{fg_open_delay_reg_input, detectr_ready_timeout_reg_input, phase_shift_reg_input, detonate_len_reg_input, trigger_len_reg_input};

struct {
	logic detonation_signal; 
	logic output_trigger;
	logic [7:0] scenario_state; 
	logic counter_out;
} output_ports;

assign output_ports = '{detonation_signal_reg_output, trigger_reg_output, scenario_state_reg_output, counter_reg_output};

enum logic [1:0] {EXPERIMENT, EXPERIMENT_PHASE, CALIBRATION, CALIBRATION_PHASE} scenario;

always @(posedge clock_reg_input) begin
	if (control_reg_history != control_reg) begin
		case (control_reg)
			EXPERIMENT: begin
				output_ports <= '{0, 0, 0, 0};
			end
			
			EXPERIMENT_PHASE: begin
				output_ports <= '{0, 0, 0, 0};
			end
			
			CALIBRATION: begin
				output_ports <= '{'z, 0, 0, 0};
			end
			
			CALIBRATION_PHASE: begin
				output_ports <= '{'z, 0, 0, 0};
			end
			
			default: begin
				output_ports <= '{'z, 'z, 'z, 'z};
			end
		
		endcase
	control_reg_history = control_reg;
	end
end

endmodule
