`timescale 1ns/1ps  

module freDetect_tb;  
  parameter CLK = 1;
  parameter TEST_DELAY = 20; 
  reg clk;  
  reg test;  
  reg rst;  
  reg fg;
	
  localparam FG_PERIOD = 10 * 1000000;
  localparam FG_OPENED = 100 * 1000; 
    
  wire[31:0] duty_cycle;
  wire [31:0] q;

  wire[31:0] duty_cycle_fg;
  wire [31:0] q_fg;

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
    
  initial  
    begin  
      fg = 1'b0;  
      forever begin
	      fg = 1'b1;  
	      #FG_OPENED; 
	      fg = 1'b0;  
	      #(FG_PERIOD - FG_OPENED);
      end
    end 
	
	fg_duration_meter fg_meter(.clock(clk), .reset(rst), .d(test), .fast_gate(fg), .q(q_fg), .duty_cycle(duty_cycle_fg));
	
endmodule  
