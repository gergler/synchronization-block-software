module fsm_test(input CLOCK_50, start_signal, fg_signal, fg_delay,
					 output detector_signal);

localparam FG_DELAY = fg_delay;
localparam DETECTOR_DELAY = 5; 
localparam TRIGGER_DELAY = 350_000; 

reg[31:0] counter = 0;

enum logic [2:0] {IDLE, FG_WAIT, FG, TRIGGER, DETECTOR_WAIT} state;

always @(posedge CLOCK_50) begin
	case (state)
		IDLE: begin
			if (start_signal) 
				state <= FG_WAIT;
		end
		
		FG_WAIT: begin
			@(posedge fg_signal)
				state <= FG_DELAY;
		end
		
		FG: begin
			counter <= 0;
			while (counter <= FG_DELAY)
				counter <= counter + 1;
			state <= TRIGGER;
		end
		
		TRIGGER: begin
			counter <= 0;
			while (counter <= TRIGGER_DELAY)
				counter <= counter + 1;
			state <= DETECTOR_WAIT;
		end
		
		DETECTOR_WAIT: begin
			counter <= 0;
			while (counter <= DETECTOR_DELAY)
				counter <= counter + 1;
			detector_signal <= 1;
			state <= IDLE;
		end
		
		default:
			state <= IDLE;
end

endmodule
