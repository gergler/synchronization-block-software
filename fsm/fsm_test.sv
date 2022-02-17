module fsm_test(input clock, reset, start_signal, fg_signal, 
				output detector_trigger);

localparam FG_DELAY = 100_000;  
localparam DETECTOR_DELAY = 5; 
localparam TRIGGER_DELAY = 350_000; 


reg[31:0] times_repeat = 10;
reg[31:0] counter = 0;

enum logic [2:0] {IDLE, FG_WAIT, FG, TRIGGER_WAIT, OUTPUT_TRIGGER} state;

always @(posedge clock or posedge reset) begin
	if (reset) begin
		state <= IDLE;
		counter <= '0;
	end
	case (state)
		IDLE: begin
			if (start_signal) 
				state <= FG_WAIT;
		end
		
		FG_WAIT: begin
			if (fg_signal)
				state <= FG;
		end
		
		FG: begin
			if (counter < FG_DELAY)
				counter <= counter + 1;
			else begin
				 state <= TRIGGER_WAIT;
				 counter <= 0;
			end
		end
		
		TRIGGER_WAIT: begin
			if (counter < TRIGGER_DELAY)
				counter <= counter + 1;
			else begin
				 state <= OUTPUT_TRIGGER;
				 counter <= 0;
			end
		end
		
		OUTPUT_TRIGGER: begin
			if (counter < DETECTOR_DELAY)
				counter <= counter + 1;
			else begin 
				 detector_trigger <= 1;
				 if (times_repeat != 0) begin 
						state <= FG;
						times_repeat <= times_repeat - 1;
						end
				 else 
						state <= IDLE;
				 counter <= 0;
			end
	  end
		
		default:
			state <= IDLE;
    endcase
end

endmodule
