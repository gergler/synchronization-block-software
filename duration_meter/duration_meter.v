module duration_meter(clock, reset, d, q, duty_cycle);  
  input clock;
  input d; 
  input reset; 
  output reg[31:0] q;
  output reg[31:0] duty_cycle;
  
  reg[31:0] duration;

  reg prev_d;
  always @(posedge clock) begin
		prev_d <= d;
		if ({prev_d, d} == 2'b01) begin
			q <= duration + 1;
			duration <= 0;
		end
		else 
			duration <= duration + 1;

		if ({prev_d, d} == 2'b10) begin
			duty_cycle <= duration + 1;
		end
	end	

endmodule  
