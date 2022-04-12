import types_pkg::*;

module fsm_experiment_phase(
	input  clock, reset,
	input  input_signals_t  in,
	input  parameters_t     par,
	output output_signals_t out
);

reg[31:0] counter = '0;
reg[1:0] start_history = 0; 
reg[1:0] phase_history = 0; 
reg[1:0] fg_history = 0; 
reg[1:0] wire_history = 0;

reg detonator_triggered_reg = '0;
reg output_trigger_reg    = '0;
assign out.output_trigger = output_trigger_reg;
assign out.detonator_triggered = detonator_triggered_reg;

enum logic [7:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, DETONATE, WIRE_TRIGGER, WAIT_PHASE_FRONT, WAIT_PHASE_DELAY, TRIGGER_PROLONG, DETECTOR_BUSY, DETECTOR_WAIT, DETECTOR_FINISHED} state;
assign out.scenario_state [7:0] = {'0, state [3:0]};

assign out.valid_scenario = 1;

always @(posedge clock) begin
	start_history[1:0] = {start_history[0], in.start}; 
    phase_history[1:0] = {phase_history[0], in.phase};
    fg_history[1:0] = {fg_history[0], in.fg_opto};
	wire_history[1:0] = {wire_history[0], in.wire_sensor};
	
	if (reset) begin
		state <= IDLE;
		{detonator_triggered_reg, output_trigger_reg, counter} <= '0;
	end else
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
			if (counter < par.FG_OPEN_DELAY)
				counter <= counter + 1;
            else begin
				state <= DETONATE;
				counter <= 0;
			end
		end
		
		DETONATE: begin
			if (counter < par.DETONATE_LEN) begin
				detonator_triggered_reg <= '1;
				counter <= counter + 1;
			end
			else begin
				detonator_triggered_reg <= '0;
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
            if (counter < par.PHASE_SHIFT)
				counter <= counter + 1;
			else begin
                output_trigger_reg <= '1;
				state <= TRIGGER_PROLONG;
                counter <= 0;
			end
		end
        
        TRIGGER_PROLONG: begin
			if (counter < par.TRIGGER_LEN) begin
				counter <= counter + 1;
			end 
            else begin
                output_trigger_reg <= 0;
				state <= DETECTOR_BUSY;
			end
		end
        
        DETECTOR_BUSY: begin
            if (~in.detector_ready) begin
                state <= DETECTOR_WAIT;
			end
        end
		
		DETECTOR_WAIT: begin
			if ((counter < par.DETECTOR_READY_TIMEOUT) && (~in.detector_ready))
				counter <= counter + 1;
			else begin
				state <= DETECTOR_FINISHED;
				output_trigger_reg <= '0;
				counter <= 0;
			end
		end
		
		DETECTOR_FINISHED: begin
            if (in.start == '0)
                state <= IDLE;
		end
		
		default:
			state <= IDLE;
    endcase
end

endmodule
