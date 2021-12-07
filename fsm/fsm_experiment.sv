module fsm_test(input CLOCK_50, start_signal, fg_signal, wire_signal,
					 output detonation_signal, detector_signal);

localparam FG_DELAY = 10_000_000;
localparam DETECTOR_DELAY = 5; 
localparam TRIGGER_DELAY = 350_000; 

reg[31:0] counter = 0;

enum logic [2:0] {IDLE, FG_WAIT, FG, DETONATE, WIRE_TRIGGER, DETECTOR_WAIT, DETECTOR_TRIGGER} state;

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
			state <= DETONATE;
		end
		
		DETONATE: begin
			detonation_signal <= 1;
			state <= WIRE_TRIGGER;
		end
		
		WIRE_TRIGGER: begin
			@(posedge wire_signal)
				state <= DETECTOR_WAIT;
		end
		
		DETECTOR_WAIT: begin
			counter <= 0;
			while (counter <= DETECTOR_DELAY)
				counter <= counter + 1;
			state <= DETECTOR_TRIGGER;
		end
		
		DETECTOR_TRIGGER: begin
			detector_signal <= 1;
			state <= IDLE;
		end
		
		default:
			state <= IDLE;
end

endmodule
