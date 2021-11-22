`timescale 1ps/1ps  

module freDetect_tb;  
  parameter CLK = 1;
  parameter TEST_DELAY = 20; 
  reg clk;  
  reg test;  
  reg rst;  
    
  wire[31:0] duty_cycle;
  wire [31:0] q;

  integer i;
    
  initial  
    begin  
      rst =1'b0;  
      #1 rst = 1'b1;  
    end  
      
  initial  
    begin  
      test = 1'b0;  
      forever begin
      	for (i = 0; i < 10; i = i+1) begin
		#20 test = ~test;
		#10 test = ~test;
	end
	for (i = 0; i < 20; i = i+1) 
		#TEST_DELAY test = ~test;
      end
    end  
      
  initial  
    begin  
      clk = 1'b0;  
      forever #CLK clk = ~clk;  
    end  
      
    duration_meter meter(.clock(clk), .reset(rst), .d(test), .q(q), .duty_cycle(duty_cycle));   
endmodule  
