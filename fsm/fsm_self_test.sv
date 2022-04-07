`timescale 1ns/10ps

module fsm_self_test(input logic clock,
					 output [7:0] state_out
);

enum logic [7:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, WAIT_PHASE_FRONT, WAIT_PHASE_DELAY, TRIGGER_PROLONG, DETECTOR_BUSY, DETECTOR_WAIT, DETECTOR_FINISHED} state;

assign state_out = state;

always @(posedge clock) begin
		state <= IDLE;
		#10us;
		state <= FG_WAIT_OPTO;
		#10us;
		state <= FG_WAIT_OPEN;
		#10us;
		state <= WAIT_PHASE_FRONT;
		#10us;
		state <= WAIT_PHASE_DELAY;
		#10us;
		state <= TRIGGER_PROLONG;
		#10us;
		state <= DETECTOR_BUSY;
		#10us;
		state <= DETECTOR_WAIT;
		#10us;
		state <= DETECTOR_FINISHED;
		#10us;
end

endmodule
