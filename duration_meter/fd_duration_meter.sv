module fd_duration_meter(clock, reset, d, fast_gate, q, duty_cycle);  
  input clock;
  input d; 
  input reset; 
  input fast_gate;
  output reg[31:0] q;
  output reg[31:0] duty_cycle;
  
  always @(posedge fast_gate) begin
    duration_meter meter(clock, reset, d, q, duty_cycle);  
  end

endmodule  
