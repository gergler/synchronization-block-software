module fsm_experiment(input clock, start_signal, fg_signal, wire_signal,
					  output detonation_signal, detector_signal);

localparam FG_DELAY = 100_000;  
localparam DETECTOR_DELAY = 5; 
localparam TRIGGER_DELAY = 350_000; 

reg[31:0] counter = 0;

enum logic [2:0] {IDLE, FG_WAIT, FG, DETONATE, WIRE_TRIGGER, DETECTOR_WAIT, DETECTOR_TRIGGER} state;

always @(posedge clock) begin
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
                state <= DETONATE;
                counter <= 0;
            end
		end
		
		DETONATE: begin
			detonation_signal <= 1;
			state <= WIRE_TRIGGER;
		end
		
		WIRE_TRIGGER: begin
			if (wire_signal)
				state <= DETECTOR_WAIT;
		end
		
		DETECTOR_WAIT: begin
			if (counter < DETECTOR_DELAY)
				counter <= counter + 1;
			else begin
                state <= DETECTOR_TRIGGER;
                counter <= 0;
            end
		end
		
		DETECTOR_TRIGGER: begin
			detector_signal <= 1;
			state <= IDLE;
		end
		
		default:
			state <= IDLE;
    endcase
end

endmodule
