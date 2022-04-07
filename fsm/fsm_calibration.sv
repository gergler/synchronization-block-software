import types_pkg::*;

module fsm_calibration(
    input  clock, reset_signal,
    input  input_signals_t  in,
    input  parameters_t     par,
    output output_signals_t out
);

localparam FG_OPEN_DELAY = par.fg_open_delay; //9_000_000/5;  
localparam TRIGGER_LEN = par.trigger_len; //100; 

reg[31:0] counter = 0;
reg[1:0] start_history = 0; 
reg[1:0] reset_history = 0; 
reg[1:0] fg_history = 0; 

logic start_signal, fg_signal;
assign start_signal = in.start_signal;
assign fg_signal = in.fg_signal;

reg output_trigger_reg    = '0;
assign out.output_trigger = output_trigger_reg;

enum logic [7:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, TRIGGER_PROLONG} state;
assign out.scenario_state = {'0, state [1:0]};

always @(posedge clock) begin
    start_history[1:0] = {start_history[0], in.start_signal}; 
    reset_history[1:0] = {reset_history[0], reset_signal};
    fg_history[1:0] = {fg_history[0], fg_signal};
    
	if (reset_history == 2'b01) begin
		state <= IDLE;
		counter <= 0;
        output_trigger <= 0;
	end
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
			if (counter < FG_OPEN_DELAY)
				counter <= counter + 1;
			else begin
				state <= TRIGGER_PROLONG;
                counter <= 0;
			end
		end
        
		TRIGGER_PROLONG: begin
			if (counter < TRIGGER_LEN) begin
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
