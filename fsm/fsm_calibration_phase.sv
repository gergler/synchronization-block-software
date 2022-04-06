module fsm_calibration_phase(
    input  logic clock, reset_signal, start_signal, fg_signal, phase_signal, 
	output logic output_trigger,
    output [2:0] scenario_state, output int counter_out
);

localparam FG_DELAY = 9_000_000/5;  
localparam TRIGGER_LEN = 100; 
localparam PHASE_SHIFT = 700/5;

reg[31:0] counter = 0;

reg[1:0] start_history = 0; 
reg[1:0] reset_history = 0; 
reg[1:0] phase_history = 0; 
reg[1:0] fg_history = 0; 

enum logic [7:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, WAIT_PHASE_FRONT, WAIT_PHASE_DELAY, TRIGGER_PROLONG} state;

assign scenario_state = state;
assign counter_out = counter;

always @(posedge clock) begin
    start_history[1:0] = {start_history[0], start_signal}; 
    reset_history[1:0] = {reset_history[0], reset_signal};
    phase_history[1:0] = {phase_history[0], phase_signal};
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
			if (counter < FG_DELAY)
				counter <= counter + 1;
			else begin
				state <= WAIT_PHASE_FRONT;
                counter <= 0;
			end
		end
		
        WAIT_PHASE_FRONT: begin
            if (phase_history == 2'b01) 
                state <= WAIT_PHASE_DELAY;
            counter <= 0;
        end
        
        WAIT_PHASE_DELAY: begin
            if (counter < PHASE_SHIFT)
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