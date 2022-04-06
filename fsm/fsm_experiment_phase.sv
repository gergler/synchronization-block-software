module fsm_experiment_phase(
		input clock, reset_signal, start_signal, fg_signal, phase_signal, wire_signal, detector_ready, 
		output detonation_signal, output_trigger, 
		output [7:0] scenario_state, output int counter_out
);

localparam FG_OPEN_DELAY = 100_000*4;    //10;  
localparam DETECTOR_READY_TIMEOUT = 7_000_000/5; //5*100; 
localparam PHASE_SHIFT = (700/5 - 1);
localparam DETONATE_LEN = 1000/5;
localparam TRIGGER_LEN = 1000/5;

reg[31:0] counter = '0;

reg[1:0] reset_history = 0; 
reg[1:0] start_history = 0; 
reg[1:0] phase_history = 0; 
reg[1:0] fg_history = 0; 
reg[1:0] wire_history = 0;

reg detonation_signal_reg = '0;
reg output_trigger_reg    = '0;

enum logic [3:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, DETONATE, WIRE_TRIGGER, WAIT_PHASE_FRONT, WAIT_PHASE_DELAY, TRIGGER_PROLONG, DETECTOR_BUSY, DETECTOR_WAIT, DETECTOR_FINISHED} state;

assign scenario_state [7:0] = { '0, state [3:0] };
assign counter_out = counter;

assign output_trigger = output_trigger_reg;
assign detonation_signal = detonation_signal_reg;

always @(posedge clock) begin
	reset_history[1:0] = {reset_history[0], reset_signal};
	start_history[1:0] = {start_history[0], start_signal}; 
    phase_history[1:0] = {phase_history[0], phase_signal};
    fg_history[1:0] = {fg_history[0], fg_signal};
	wire_history[1:0] = {wire_history[0], wire_signal};
	
	if (reset_history == 2'b01) begin
		state <= IDLE;
		{detonation_signal_reg, output_trigger_reg, counter} <= '0;
	end
    
	case (state)
		IDLE: begin
			if (start_history == 2'b01) 
				state <= FG_WAIT_OPTO;
		end
		
		FG_WAIT_OPTO: begin
			if (fg_history == 2'b01) begin
				state <= FG_WAIT_OPEN;
                counter <= '0;
            end
		end
		
		FG_WAIT_OPEN: begin
			if (counter < FG_OPEN_DELAY)
				counter <= counter + 1;
            else begin
				state <= DETONATE;
				counter <= 0;
			end
		end
		
		DETONATE: begin
			if (counter < DETONATE_LEN) begin
				detonation_signal_reg <= '1;
				counter <= counter + 1;
			end
			else begin
				detonation_signal_reg <= '0;
				state <= WIRE_TRIGGER;
                counter <= 0;
			end
	   end
	
		WIRE_TRIGGER: begin
			if (wire_history == 2'b01) begin
                state <= WAIT_PHASE_FRONT;
			end
		end
        
        WAIT_PHASE_FRONT: begin
            if (phase_history == 2'b01) begin
                state <= WAIT_PHASE_DELAY;
				counter <= 0;
			end
        end
        
        WAIT_PHASE_DELAY: begin
            if (counter < PHASE_SHIFT)
				counter <= counter + 1;
			else begin
                output_trigger_reg <= '1;
				state <= TRIGGER_PROLONG;
                counter <= 0;
			end
		end
        
        TRIGGER_PROLONG: begin
			if (counter < TRIGGER_LEN) begin
				counter <= counter + 1;
			end 
            else begin
                output_trigger_reg <= 0;
				state <= DETECTOR_BUSY;
			end
		end
        
        DETECTOR_BUSY: begin
            if (~detector_ready) begin
                state <= DETECTOR_WAIT;
			end
        end
		
		DETECTOR_WAIT: begin
			if ((counter < DETECTOR_READY_TIMEOUT) && (~detector_ready))
				counter <= counter + 1;
			else begin
				state <= DETECTOR_FINISHED;
				output_trigger_reg <= '0;
				counter <= 0;
			end
		end
		
		DETECTOR_FINISHED: begin
            if (start_signal == '0)
                state <= IDLE;
		end
		
		default:
			state <= IDLE;
    endcase
end

endmodule
