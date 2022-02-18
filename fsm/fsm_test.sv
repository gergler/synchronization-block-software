module fsm_test(input clock, reset, start_signal, fg_signal, 
				output output_trigger, output [2:0] scenario_state, output int counter_);

localparam FG_DELAY = 400_000;  
localparam TRIGGER_LEN = 100; 

reg[31:0] counter = '0;

enum logic [2:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, TRIGGER_PROLONG} state;

assign scenario_state = state;
assign counter_ = counter;

always @(posedge clock) begin
	if (reset) begin
		state <= IDLE;
		counter <= 0;
	end
	case (state)
		IDLE: begin
			if (start_signal) 
				state <= FG_WAIT_OPTO;
		end
		
		FG_WAIT_OPTO: begin
			if (fg_signal)
				state <= FG_WAIT_OPEN;
            counter <= 0;
		end
		
		FG_WAIT_OPEN: begin
			if (counter < FG_DELAY)
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
