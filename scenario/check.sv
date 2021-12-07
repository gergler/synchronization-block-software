module check(fast_gate, phase_signal, check_condition);
  output check_condition;
  input fast_gate, phase_signal;
  
  assign check_condition = ~(fast_gate & phase_signal);
  
endmodule
