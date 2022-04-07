`timescale 1ns/10ps

module fsm_self_test(input clock,
					 output output_signals_t out
//					 output [7:0] state_out
);

enum logic [7:0] {IDLE, FG_WAIT_OPTO, FG_WAIT_OPEN, WAIT_PHASE_FRONT, WAIT_PHASE_DELAY, TRIGGER_PROLONG, DETECTOR_BUSY, DETECTOR_WAIT, DETECTOR_FINISHED} state;

assign out.scenario_state = {'0, state [3:0]};

always_comb begin
	for (int i = 0; i < variants.num(); i++) begin
		state <= i;
		#10us;
	end
end

endmodule
