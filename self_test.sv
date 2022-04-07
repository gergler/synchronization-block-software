`timescale 1ns/10ps

module fsm_self_test(input logic clock, fg_signal, fg_delay,
				 output int counter_out
);

reg[31:0] counter = 0;
assign counter_out = counter;

reg[31:0] FG_DELAY = 0;
assign FG_DELAY = fg_delay;

reg[1:0] fg_history = 0; 

enum logic [7:0] {FG_WAIT_OPTO, FG_WAIT_OPEN} state;

always @(posedge clock) begin
	fg_history[1:0] = {fg_history[0], fg_signal};
	
	case (state)
		FG_WAIT_OPTO: begin
			if (fg_history == 2'b01)
				state <= FG_WAIT_OPEN;
			counter <= 0;
		end
		
		FG_WAIT_OPEN: begin
			if (counter < FG_DELAY)
				counter <= counter + 1;
			else begin
				state <= FG_WAIT_OPTO;
				counter <= 0;
			end
		end
		
		default:
			state <= FG_WAIT_OPTO;
end    
	
	
endmodule
