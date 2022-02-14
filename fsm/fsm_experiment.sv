`timescale 10ns/1ps

module fsm_experiment(input clock, reset, start_signal, fg_signal, wire_signal, detector_ready,
					  output detonation_signal, output_trigger);

localparam FG_DELAY = 100_000*4;//10;  
localparam DETECTOR_READY_TIMEOUT = 5*100; 
localparam TRIGGER_DELAY = 350_000; 

reg[31:0] counter = '0;

enum logic [2:0] {IDLE, FG_WAIT, FG, DETONATE, WIRE_TRIGGER, DETECTOR_WAIT, DETECTOR_FINISHED} state;

reg detonation_signal_reg = '0;
reg output_trigger_reg    = '0;

assign output_trigger = output_trigger_reg;
assign detonation_signal = detonation_signal_reg;

always @(posedge clock or posedge reset) begin
	if (reset) begin
		state <= IDLE;
		{detonation_signal_reg, output_trigger_reg, counter} <= '0;
	end
	case (state)
		IDLE: begin
			if (start_signal) 
				state <= FG_WAIT;
//            {detonation_signal_reg, output_trigger_reg} <= '0;
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
			detonation_signal_reg <= 1;
			#50;
			detonation_signal_reg <= 0;
			state <= WIRE_TRIGGER;
	   end
	
		WIRE_TRIGGER: begin
			if (wire_signal) begin
				 output_trigger_reg <= 1;
			state <= DETECTOR_WAIT;
			end
		end
		
		DETECTOR_WAIT: begin
			if ((counter < DETECTOR_READY_TIMEOUT) && (~detector_ready))
				counter <= counter + 1;
			else begin
				 state <= DETECTOR_FINISHED;
				 output_trigger_reg <= 0;
				 counter <= 0;
			end
		end
		
		DETECTOR_FINISHED: begin
			state <= IDLE;
		end
		
		default:
			state <= IDLE;
    endcase
end

endmodule
