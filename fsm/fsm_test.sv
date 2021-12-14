module fsm_test(input clock, start_signal, fg_signal, 
				output detector_signal);

localparam FG_DELAY = 100_000;  
localparam DETECTOR_DELAY = 5; 
localparam TRIGGER_DELAY = 350_000; 

reg[31:0] counter = 0;

enum logic [2:0] {IDLE, FG_WAIT, FG, TRIGGER, DETECTOR_WAIT} state;

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
                state <= TRIGGER;
                counter <= 0;
            end
		end
		
		TRIGGER: begin
			if (counter < TRIGGER_DELAY)
				counter <= counter + 1;
			else begin
                state <= DETECTOR_WAIT;
                counter <= 0;
            end
		end
		
		DETECTOR_WAIT: begin
			if (counter < DETECTOR_DELAY)
				counter <= counter + 1;
			else begin 
                detector_signal <= 1;
                state <= IDLE;
                counter <= 0;
            end
        end
		
		default:
			state <= IDLE;
    endcase
end

endmodule
