import types_pkg::*;

module fsm_self_test(
    input  clock, reset,
    input  input_signals_t  in,
    output output_signals_t out
);

reg[63:0] counter = '0;

enum logic {IDLE, TEST} state;
assign out.scenario_state = {counter[6:0], state}; //{counter[34:28], state};

always @(posedge clock) begin
    
	if (reset) begin
		state   <= IDLE;
		counter <= '0;
	end else
	case (state)
		IDLE: begin
            counter <= '0;
			if (in.start) 
				state <= TEST;
		end
		
		TEST: begin
            counter <= counter + 64'd1;
		end
        
    endcase
end

endmodule
