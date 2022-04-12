import types_pkg::*;

module fsm_calibration(
    input  clock, reset,
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);

reg[31:0] counter = 0;
reg[1:0] start_history = 0; 
reg[1:0] fg_history = 0; 

logic output_trigger;
assign out.output_trigger = output_trigger;

enum logic [7:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, TRIGGER_PROLONG} state;
assign out.scenario_state = {'0, state [1:0]};

assign out.valid_scenario = 1;

always @(posedge clock) begin
    start_history[1:0] = {start_history[0], in.start}; 
    fg_history[1:0] = {fg_history[0], in.fg_opto};
    
	if (reset) begin
		state <= IDLE;
		counter <= 0;
        output_trigger <= 0;
	end else
	case (state)
		IDLE: begin
			if (start_history == 2'b01) 
				state <= FG_WAIT_OPTO;
		end
		
		FG_WAIT_OPTO: begin
			if (fg_history == 2'b01)
				state <= FG_WAIT_OPEN;
            counter <= 0;
		end
		
		FG_WAIT_OPEN: begin
			if (counter < par.FG_OPEN_DELAY)
				counter <= counter + 1;
			else begin
				state <= TRIGGER_PROLONG;
                counter <= 0;
			end
		end
        
		TRIGGER_PROLONG: begin
			if (counter < par.TRIGGER_LEN) begin
				counter <= counter + 1;
                output_trigger <= 1;
			end 
            else begin
                output_trigger <= 0;
				state <= IDLE;
			end
		end
		
		default:
			state <= IDLE;
    endcase
end

endmodule
