module check(fast_lock, phase, out_value);
  output out_value;
  input fast_lock, phase;
  
  assign out_value = ~(fast_lock & phase);
  
endmodule
